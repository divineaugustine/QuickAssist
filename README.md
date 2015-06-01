# QuickAssist #
An addin to easily search and open files for visual studio and notepad ++

## Screen shots ##
Loaded in notepad++
<div style="img-align:center"><img src ="Docs/screenshots/NPP_1.png" /></div>

Loaded in vs 2012
<div style="img-align:center"><img src ="Docs/screenshots/VS_GUI.png" /></div>

#Why Quick Assist#
I found it difficult to navigate to files and functions quickly when working with a very large code base. We were relying on make files hence no project files even though we were using visual studio as an editor. To make it worse, the folder structure was hard to remember with lots of sub folders.
Below are the requirements i was looking for in the tool.

1. Simple, no-install utility that can easily be integrated with visual studio and notepad ++
2. Total control over what types of files are getting indexed and from where.
3. Index should be reusable in another machine
4. Indexing should work with a relative path. Useful when working with multiple clearcase views. So most of the time we can live with the same index unless a file addition/removal has been made. Or in scenarios where the code will be stored in a pendrive.

# Features #

1. Toggle source/header
2. Search file
3. Search files using '&'
4. Jump to a specific file by text selection
5. Show explorer context menu

Read [Docs/Features.md](docs/Features.md) for more details and screenshots

#How to configure#

1. Run the AddPath.vbs to update the registry with path

2.1 <Notepad++>
Copy QuickAssist_npp.dll to Notepad++\Plugins folder 
2.2 <Visual Studio>
Edit QuickAssist.AddIn with adding the complete assembly path.

3. Open notepad++/VS click settings icon.
<div style="img-align:center"><img src ="Docs/screenshots/settings.png" /></div>

3.1 Select the base folder by '...'
3.2 Create the new repository by clicking '+'
3.2 Give a name and click 'Create' 
3.3 Change settings, use combo to select multiple options
3.4 Click 'Update' to save and close the dialog
3.5 Click 'Sync Index' to start the indexing
3.6 Click 'Reload' when the indexing is over

#Components#
<div style="img-align:center"><img src ="Docs/screenshots/Components.png" /></div>
