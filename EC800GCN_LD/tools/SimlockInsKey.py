#!/usr/bin/env python
from struct import pack, unpack
import subprocess,os,sys,struct,array
from Crypto.PublicKey import RSA
from Crypto.Util.number import long_to_bytes
#cpath=os.getcwd()
#cpath="/home/hesheng.lv/projects/8850/Bin/InsertSimlocKey/"
KeyType='RSA2048'
MagicNum='SIMLOCK_MAGIC_S'
KeyUpDir = "/SimlocKeys/"
KeyIDTable = [0x1011, 0x1012, 0x1013, 0x1014, 0x1015]
KeysDirs = ["authstartkey", "authendkey", "simlockey", "imeikey", "reskey"]

def GetKeyFiles(fdir):
	kf = ""
	fls=os.listdir(fdir)
	j = 0
	for i in fls:
		if os.path.splitext(i)[1] == ".pem":
			#print "Find key: " + i
			if kf == "":
				kf = i
			j = j + 1
		if j > 1:
			kf = ""
	return j, kf

def get_head_offset(tos_img,tos_len):
	secpos = tos_img.find(MagicNum)
	if secpos < 0:
		print ("there is no head!!!")
		return 0
	else:
		print ("Find magic num: " + MagicNum)
		return  secpos

def InsertKey(kpath,kid,img):
	f = open(kpath, 'rb')
	if f < 0:
		print ("Can\'t find key: \""+kpath+"\"")
		return 0
	else:
		k = RSA.importKey(f.read())
		f.close
		Mlen = (k.publickey().size()+7)/8
		Nlen = len(long_to_bytes(k.publickey().n))
		Elen = len(long_to_bytes(k.publickey().e))
		img.write('%s' % struct.pack("<I", kid))
		img.write('%s' % struct.pack("<I", Mlen))
		img.write(KeyType)
		img.seek(1,1)
		img.write('%s' % struct.pack("<I", Nlen))
		for x in array.array("B", long_to_bytes(k.publickey().n)):
			data_n = struct.pack("<B",x)
			img.write(data_n)
		img.seek(512-Nlen, 1)
		img.write('%s' % struct.pack("<I", Elen))
		for x in array.array("B", long_to_bytes(k.publickey().e)):
			data_e = struct.pack("<B",x)
			img.write(data_e)
		img.seek(512-Elen, 1)
		#print ("Insert key["+hex(kid)+"]:\""+os.path.basename(kpath)+"\" success")
		return	1

def main():
	filename = sys.argv[1]
	cpath = sys.argv[2]
	#print(sys.argv)
	if os.path.exists(filename):
		f1 = open(filename,'rb+')
		bindata = f1.read()
		bin_len = len(bindata)
		offset = get_head_offset(bindata,bin_len)
		if offset == 0:
			print ("Can\'t find head magic num")
		else:
			print ("Magic Offset is " + bytes(offset))
			f1.seek(offset+16,0)
			for n in range(0,5):
				knum, kfile = GetKeyFiles(cpath+KeyUpDir+KeysDirs[n])
				if knum == 1:
					InsertKey(cpath+KeyUpDir+KeysDirs[n]+"/"+kfile, KeyIDTable[n], f1)
			print ("Insert simlock key success")
		f1.close
	else:
		print ("Cp bin is not accessible!!!")

if __name__ == "__main__":
	main()
