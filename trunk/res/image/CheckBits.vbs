Const wiaIDUnknown = "{00000000-0000-0000-0000-000000000000}"
Const wiaFormatBMP = "{B96B3CAB-0728-11D3-9D7B-0000F81EF32E}"

Set oImg = CreateObject("Wia.ImageFile")

Set oFSO = CreateObject("Scripting.FileSystemObject")

For Each oFile In oFSO.GetFolder(".").Files
	
    If UCase(oFSO.GetExtensionName(oFile.name)) = "BMP" Then
		
		If oImg.FormatID = wiaIDUnknown Then
            ' The file isn't an image file
		Else
            If oImg.PixelDepth <> 24 Then
                oImg.LoadFile(oFile)
			
                Wscript.Echo "Is BMP: " & (oImg.FormatID = wiaFormatBMP)
                Wscript.Echo "Color depth: " & oImg.PixelDepth
            End If
		End If
	End If

Next