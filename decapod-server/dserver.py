import cherrypy
import re,string,os,sys,shutil,glob
from numpy import *
from pylab import imread,imshow

index = """
<a href="startProject?id=temp">startProject temp</a><br>
<a href="capture?id=temp&image=img000">capture temp/img000</a><br>
<a href="dewarp?id=temp&image=img000">dewarp temp/img000</a><br>
<a href="rename?id=temp&image=img000&new=out000">rename temp/img000 temp/out000</a><br>
<a href="delete?id=temp&image=img000">delete temp/img000</a><br>
"""

camcap_cmd = \
    "gst-launch v4l2src num-buffers=1 device=%s ! " + \
    "video/x-raw-yuv,width=%d,height=%d ! " + \
    "ffmpegcolorspace ! jpegenc ! filesink location='%s'"

def camcap(file,device="/dev/video0",width=640,height=480):
    os.system(camcap_cmd%(device,width,height,file))

def check_id(id):
    if id is None:
        raise "no identifier given"
    if re.search(r'[^a-zA-Z0-9_-]',id)>=0:
        raise "bad identifier"
def check_pattern(id):
    if id is None:
        raise "no identifier given"
    if re.search(r'[^a-zA-Z0-9_*?-]',id)>=0:
        raise "bad identifier"

class DecapodServer(object):
    @cherrypy.expose
    def index(self):
        return index
    @cherrypy.expose
    def startProject(self,id=None):
        check_id(id)
        os.mkdir(id)
    @cherrypy.expose
    def capture(self,id=None,image=None):
        check_id(id)
        check_id(image)
        base = os.path.join(id,image)
        camcap("%s.raw.1.jpg" % base)
        camcap("%s.raw.2.jpg" % base)
        jpeg = open("%s.raw.1.jpg"%base,"rb").read()
        cherrypy.response.headers['Content-type'] = 'image/jpeg'
        return [jpeg]
    @cherrypy.expose
    def dewarp(self,id=None,image=None):
        check_id(id)
        check_id(image)
        base = os.path.join(id,image)
        image1 = imread("%s.raw.1.jpg" % base)
        image2 = imread("%s.raw.2.jpg" % base)
        os.system("convert %s.raw.1.jpg %s.png"%(base,base))
        dewarped = open("%s.png"%base).read()
        cherrypy.response.headers['Content-type'] = 'image/png'
        return [dewarped]
    @cherrypy.expose
    def glob(self,id=None,pattern="*"):
        check_id(id)
        check_pattern(pattern)
        result = ""
        for file in sorted(glob.glob(os.path.join(id,pattern))):
            file = file[len(id)+1:]
            result += file+"\n"
        cherrypy.response.headers['Content-type'] = 'text/plain'
        return result
    @cherrypy.expose
    def delete(self,id=None,image=None):
        check_id(id)
        check_id(image)
        result = ""
        prefix = os.path.join(id,image)
        for file in glob.glob(prefix+".*"):
            newfile = new+file[len(prefix):]
            os.unlink(file)
            result += file+"\n"
        cherrypy.response.headers['Content-type'] = 'text/plain'
        return result
    @cherrypy.expose
    def rename(self,id=None,image=None,new=None):
        check_id(id)
        check_id(image)
        check_id(new)
        result = ""
        prefix = os.path.join(id,image)
        for file in glob.glob(prefix+".*"):
            newfile = new+file[len(prefix):]
            os.rename(file,newfile)
            result += file + "\t" + newfile + "\n"
        cherrypy.response.headers['Content-type'] = 'text/plain'
        return result
    @cherrypy.expose
    def makePdf(self,id=None):
        check_id(id)
        result = ""
        prefix = os.path.join(id,"????.png")
        os.system("convert %s/[0-9][0-9][0-9][0-9].png %s/output.pdf"%(id,id))
        result = open("%s/output.pdf"%id).read()
        cherrypy.response.headers['Content-type'] = 'application/pdf'
        return result
    #
    # asynchronous methods
    #
    @cherrypy.expose
    def startDewarp(self,id=None,pattern="*.raw.1.jpg"):
        check_id(id)
        check_pattern(pattern)
        pattern = os.path.join(id,pattern)
        pid = os.fork()
        if pid==0:
            for file in glob.glob(pattern):
                base = re.sub(r'.raw.1.jpg$','',file)
                os.system("convert %s.raw.1.jpg %s.png"%(base,base))
        result = "%d"%pid
        return result
    @cherrypy.expose
    def checkDewarp(self,id=None,pid=None):
        pid = int(pid)
        assert pid>0 and pid<999999
        if os.system("ps %d | grep -s %d" % (pid,pid))==0:
            return "running"
        else:
            return "done"
    @cherrypy.expose
    def startPdf(self,id=None):
        check_id(id)
        result = ""
        prefix = os.path.join(id,"????.png")
        try: os.unlink("%s/output.pdf"%id)
        except: pass
        pid = os.fork()
        if pid==0:
            os.system("convert %s/[0-9][0-9][0-9][0-9].png %s/output.pdf"%(id,id))
        return "%d"%pid
    @cherrypy.expose
    def checkPdf(self,id=None,pid=None):
        pid = int(pid)
        assert pid>0 and pid<999999
        if os.system("ps %d | grep -s %d" % (pid,pid))==0:
            return "running"
        else:
            return "done"
    @cherrypy.expose
    def getPdf(self,id=None):
        check_id(id)
        result = open("%s/output.pdf"%id).read()
        cherrypy.response.headers['Content-type'] = 'application/pdf'
        return result
    
cherrypy.quickstart(DecapodServer())
