
DevAssistIFps.dll: dlldata.obj DevAssistIF_p.obj DevAssistIF_i.obj
	link /dll /out:DevAssistIFps.dll /def:DevAssistIFps.def /entry:DllMain dlldata.obj DevAssistIF_p.obj DevAssistIF_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del DevAssistIFps.dll
	@del DevAssistIFps.lib
	@del DevAssistIFps.exp
	@del dlldata.obj
	@del DevAssistIF_p.obj
	@del DevAssistIF_i.obj
