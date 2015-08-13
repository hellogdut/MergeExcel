'WalkFolders.vbs
't0nsha<liaodunxia#gmail.com>
'2010/3/20

'得到脚本文件所在的当前目录
Function GetCurrentFolderFullPath
	Set fso = CreateObject("Scripting.FileSystemObject")
	GetCurrentFolderFullPath = fso.GetParentFolderName(WScript.ScriptFullName)
End Function



Function toCsv(src,dest)
	csv_format = 6

	Set objFSO = CreateObject("Scripting.FileSystemObject")
	src_file = objFSO.GetAbsolutePathName(src)
	dest_file = objFSO.GetAbsolutePathName(dest)

	Dim oBook
	Set oBook = oExcel.Workbooks.Open(src_file)

	oBook.Worksheets("Sheet1").Activate 

	oExcel.Displayalerts=false
	oBook.SaveAs dest_file, -4158
	oExcel.Displayalerts=true
	oBook.Close False
	oExcel.Quit
End Function

Function openExcel(src,dest)
	csv_format = 6

	Set objFSO = CreateObject("Scripting.FileSystemObject")
	src_file = objFSO.GetAbsolutePathName(src)
	dest_file = objFSO.GetAbsolutePathName(dest)

	Dim oBook
	Set oBook = oExcel.Workbooks.Open(src_file)

	oBook.Worksheets("Sheet1").Activate 

	oExcel.Displayalerts=false
	oBook.SaveAs dest_file
	oExcel.Displayalerts=true
	oBook.Close False
	oExcel.Quit
End Function





Function FilesTree(sPath)  
  Dim i : i=0
  on error resume Next
  Set oFso = CreateObject("Scripting.FileSystemObject")   
  Set oFolder = oFso.GetFolder(sPath)        
  Set oFiles = oFolder.Files   
  For Each oFile In oFiles 
  		src = oFile.Path
  		dest = Left(oFile.Path,len(oFile.Path)-3) + "txt"
  		call toCsv(src,dest)
  Next 
End Function  

'vbs递归遍历当前目录下的所有文件夹和子文件夹
Function GetSubFolders(currentFolderFullPath)
	Set fso = CreateObject("Scripting.FileSystemObject")
	Set currentFolder = fso.GetFolder(currentFolderFullPath)
	Set subFolderSet = currentFolder.SubFolders
	For Each subFolder in subFolderSet
		call FilesTree(subFolder)
	Next
End Function


if WScript.Arguments.Count < 1 Then
WScript.Echo "Please specify the source and the destination files. Usage: ExcelToCsv <xls/xlsx source folder>"
Wscript.Quit
End If

Dim oExcel
Set oExcel = CreateObject("Excel.Application")

GetSubFolders(Wscript.Arguments.Item(0))

