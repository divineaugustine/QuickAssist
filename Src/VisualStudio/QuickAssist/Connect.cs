using System;
using Extensibility;
using EnvDTE;
using EnvDTE80;
using Microsoft.VisualStudio.CommandBars;
using System.Resources;
using System.Reflection;
using System.Globalization;
using System.Windows.Forms;
using System.IO;
using System.Drawing;

namespace QuickAssist
{
	/// <summary>The object for implementing an Add-in.</summary>
	/// <seealso class='IDTExtensibility2' />
	public class Connect : IDTExtensibility2, IDTCommandTarget
	{


		/// <summary>Implements the constructor for the Add-in object. Place your initialization code within this method.</summary>
		public Connect()
		{
		}

		/// <summary>Implements the OnConnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being loaded.</summary>
		/// <param term='application'>Root object of the host application.</param>
		/// <param term='connectMode'>Describes how the Add-in is being loaded.</param>
		/// <param term='addInInst'>Object representing this Add-in.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnConnection(object application, ext_ConnectMode connectMode, object addInInst, ref Array custom)
		{
			_applicationObject = (DTE2)application;
			_addInInstance = (AddIn)addInInst;
			if(connectMode == ext_ConnectMode.ext_cm_UISetup)
			{
				object []contextGUIDS = new object[] { };

				Commands2 commands = (Commands2)_applicationObject.Commands;
				string toolsMenuName = "Tools";

				//Place the command on the tools menu.
				//Find the MenuBar command bar, which is the top-level command bar holding all the main menu items:
				Microsoft.VisualStudio.CommandBars.CommandBar menuBarCommandBar = ((Microsoft.VisualStudio.CommandBars.CommandBars)_applicationObject.CommandBars)["MenuBar"];
                // Microsoft.VisualStudio.CommandBars.CommandBar QuickLaunchCommand = null;

                //try
                //{
                //    QuickLaunchCommand = ((Microsoft.VisualStudio.CommandBars.CommandBars)_applicationObject.CommandBars)["QuickAssist"];
                //}
                //catch (SystemException ex)
                //{ 
                //}

                //if (QuickLaunchCommand == null)
                //{
                //    QuickLaunchCommand = ((Microsoft.VisualStudio.CommandBars.CommandBars)_applicationObject.CommandBars).Add("QuickAssist", 0, vsCommandBarType.vsCommandBarTypeToolbar, 1);
                //    QuickLaunchCommand.Visible = true;
                //}

				//Find the Tools command bar on the MenuBar command bar:
				 CommandBarControl toolsControl = menuBarCommandBar.Controls[toolsMenuName];
				 CommandBarPopup toolsPopup = (CommandBarPopup)toolsControl;


                 CommandBars cmdBars = (CommandBars)_applicationObject.CommandBars;
                 CommandBar myCommandBar = cmdBars.Add( "QuickAssist", MsoBarPosition.msoBarTop, false, false );
                 myCommandBar.Visible = true;



                ////This try/catch block can be duplicated if you wish to add multiple commands to be handled by your Add-in,
                ////  just make sure you also update the QueryStatus/Exec method to include the new command names.
                 // contextGUIDS = MyAddCommand(contextGUIDS, commands, toolsPopup.CommandBar, "DA_ToggleFile", "Toggle File", "Toggles between cpp/header");

                 
                 
                 
                 contextGUIDS = MyAddCommand(contextGUIDS, commands, myCommandBar, "DA_ContextMenu", "Shell context menu", "Opens explorer context menu", 5);
                 contextGUIDS = MyAddCommand(contextGUIDS, commands, myCommandBar, "DA_Settings", "Settings", "Opengs dev assist settings", 4);
                 contextGUIDS = MyAddCommand(contextGUIDS, commands, myCommandBar, "DA_SearchFile", "Search File", "Searches for the given file", 3);
                 contextGUIDS = MyAddCommand(contextGUIDS, commands, myCommandBar, "DA_ToggleFile", "Toggle File", "Toggles between cpp/header", 2);

                //try
                //{
                //    //Add a command to the Commands collection:
                //    Command command = commands.AddNamedCommand2(_addInInstance, "SearchFile22", "Search File22", "Executes the command for QuickAssist",
                //        true, 59, ref contextGUIDS, (int)vsCommandStatus.vsCommandStatusSupported + (int)vsCommandStatus.vsCommandStatusEnabled, (int)vsCommandStyle.vsCommandStylePictAndText, vsCommandControlType.vsCommandControlTypeButton);

                //    //Add a control for the command to the tools menu:
                //    if ((command != null) && (toolsPopup != null))
                //    {
                //        command.AddControl(toolsPopup.CommandBar, 1);
                //    }
                //}
                //catch (System.ArgumentException)
                //{
                //    //If we are here, then the exception is probably because a command with that name
                //    //  already exists. If so there is no need to recreate the command and we can 
                //    //  safely ignore the exception.
                //}
			}
		}

        private object[] MyAddCommand(object[] contextGUIDS, Commands2 commands, 
            CommandBar /*CommandBarPopup*/ toolsPopup, string strName,
            string strButtonText, string strToolTip, int bitmapnum)
        {
            try
            {
                //Add a command to the Commands collection:
                Command command = commands.AddNamedCommand2(_addInInstance, strName, strButtonText, strToolTip,
                    false/*true*/, bitmapnum, ref contextGUIDS, 
                    (int)vsCommandStatus.vsCommandStatusSupported + (int)vsCommandStatus.vsCommandStatusEnabled, 
                    /*(int)vsCommandStyle.vsCommandStylePictAndText, */
                    (int)vsCommandStyle.vsCommandStylePict,
                    vsCommandControlType.vsCommandControlTypeButton);

                //Add a control for the command to the tools menu:
                if ((command != null) && (toolsPopup != null))
                {
                    command.AddControl(/*toolsPopup.*/toolsPopup, 1);
                }
            }
            catch (System.ArgumentException)
            {
                //If we are here, then the exception is probably because a command with that name
                //  already exists. If so there is no need to recreate the command and we can 
                //  safely ignore the exception.
            }
            return contextGUIDS;
        }

		/// <summary>Implements the OnDisconnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being unloaded.</summary>
		/// <param term='disconnectMode'>Describes how the Add-in is being unloaded.</param>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnDisconnection(ext_DisconnectMode disconnectMode, ref Array custom)
		{
		}

		/// <summary>Implements the OnAddInsUpdate method of the IDTExtensibility2 interface. Receives notification when the collection of Add-ins has changed.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />		
		public void OnAddInsUpdate(ref Array custom)
		{
		}

		/// <summary>Implements the OnStartupComplete method of the IDTExtensibility2 interface. Receives notification that the host application has completed loading.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnStartupComplete(ref Array custom)
		{
		}

		/// <summary>Implements the OnBeginShutdown method of the IDTExtensibility2 interface. Receives notification that the host application is being unloaded.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnBeginShutdown(ref Array custom)
		{
		}
		
		/// <summary>Implements the QueryStatus method of the IDTCommandTarget interface. This is called when the command's availability is updated</summary>
		/// <param term='commandName'>The name of the command to determine state for.</param>
		/// <param term='neededText'>Text that is needed for the command.</param>
		/// <param term='status'>The state of the command in the user interface.</param>
		/// <param term='commandText'>Text requested by the neededText parameter.</param>
		/// <seealso class='Exec' />
		public void QueryStatus(string commandName, vsCommandStatusTextWanted neededText, ref vsCommandStatus status, ref object commandText)
		{
            // commandText = "commandText??";
            // status = (vsCommandStatus)vsCommandStatus.vsCommandStatusUnsupported;
            // return;
			if(neededText == vsCommandStatusTextWanted.vsCommandStatusTextWantedNone)
			{
				 if(commandName == "QuickAssist.Connect.DA_ToggleFile")
				 {
                     status = (vsCommandStatus)vsCommandStatus.vsCommandStatusUnsupported ;
                     if (null != _applicationObject.ActiveDocument && null != _applicationObject.ActiveDocument.Name)
                     {
                         status = (vsCommandStatus)vsCommandStatus.vsCommandStatusSupported | vsCommandStatus.vsCommandStatusEnabled;
                     }
				 }
                 else if (commandName == "QuickAssist.Connect.DA_ContextMenu")
                 {
                     status = (vsCommandStatus)vsCommandStatus.vsCommandStatusUnsupported;
                     if (null != _applicationObject.ActiveDocument && null != _applicationObject.ActiveDocument.Name)
                     {
                         status = (vsCommandStatus)vsCommandStatus.vsCommandStatusSupported | vsCommandStatus.vsCommandStatusEnabled;
                     }
                 }
                 else
                 {
                     status = (vsCommandStatus)vsCommandStatus.vsCommandStatusSupported | vsCommandStatus.vsCommandStatusEnabled;
                 }
                 return;
			}
		}
        private void OpenFile()
        {
            string currentText = System.String.Empty;
            try
            {
                TextSelection sel = (TextSelection)_applicationObject.ActiveDocument.Selection;
                currentText = sel.Text;
            }
            catch(Exception ext)
            { 
            }

            DevAssistIFAdapter inst = new DevAssistIFAdapter();
            string strFileOut = inst.OpenFile(currentText);
            if (strFileOut.Length > 0)
            {
                _applicationObject.ItemOperations.OpenFile(strFileOut);
            }
        }

        private void ToggleFile()
        {
            string currentFile = System.String.Empty;
            try
            {
                currentFile = _applicationObject.ActiveDocument.Name;
            }
            catch (Exception ext)
            {
            }
            if (System.String.Empty == currentFile)
            {
                return;
            }

            DevAssistIFAdapter inst = new DevAssistIFAdapter();
            string strFileOut = inst.ToggleFile(currentFile);
            if( strFileOut.Length > 0 )
            {
                _applicationObject.ItemOperations.OpenFile(strFileOut);
            }
        }


        private void OpenSettings()
        {
            DevAssistIFAdapter inst = new DevAssistIFAdapter();
            inst.ShowSettings();
        }

        private void ShowShellContext()
        {
            string currentFile = System.String.Empty;
            try
            {
                currentFile = _applicationObject.ActiveDocument.FullName;
            }
            catch (Exception ext)
            {
            }

            if (System.String.Empty != currentFile)
            {
                Peter.ShellContextMenu ctxMnu = new Peter.ShellContextMenu();
                FileInfo[] arrFI = new FileInfo[1];
                arrFI[0] = new FileInfo(currentFile);
                Cursor cur = new Cursor(Cursor.Current.Handle);

                // Point pt = new Point(50, 50);
                ctxMnu.ShowContextMenu(arrFI, Cursor.Position);
            }
        }

		/// <summary>Implements the Exec method of the IDTCommandTarget interface. This is called when the command is invoked.</summary>
		/// <param term='commandName'>The name of the command to execute.</param>
		/// <param term='executeOption'>Describes how the command should be run.</param>
		/// <param term='varIn'>Parameters passed from the caller to the command handler.</param>
		/// <param term='varOut'>Parameters passed from the command handler to the caller.</param>
		/// <param term='handled'>Informs the caller if the command was handled or not.</param>
		/// <seealso class='Exec' />
		public void Exec(string commandName, vsCommandExecOption executeOption, ref object varIn, ref object varOut, ref bool handled)
		{
			handled = false;
			if(executeOption == vsCommandExecOption.vsCommandExecOptionDoDefault)
			{
				if(commandName == "QuickAssist.Connect.DA_Settings")
				{
                    OpenSettings();
					handled = true;
					return;
				}
                else if(commandName == "QuickAssist.Connect.DA_ToggleFile")
				{
                    ToggleFile();
                    handled = true;
					return;
				}else if(commandName == "QuickAssist.Connect.DA_SearchFile")
				{
                    OpenFile();
                    handled = true;
					return;
				}else if(commandName == "QuickAssist.Connect.DA_ContextMenu")
				{
                    ShowShellContext();
					handled = true;
					return;
				}
			}
		}
		private DTE2 _applicationObject;
		private AddIn _addInInstance;
	}
}