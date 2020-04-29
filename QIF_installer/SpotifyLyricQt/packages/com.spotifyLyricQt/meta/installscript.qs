/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

function Component()
{
    //安装在Program File文件下
    var programFiles = installer.environmentVariable("ProgramW6432");
    if (programFiles !== "")
    installer.setValue("TargetDir", programFiles + "/Spotify LyricQt");
}

Component.prototype.isDefault = function()
{
    // select the component by default
    return true;
}

Component.prototype.createOperations = function()
{
	// call default implementation to actually install README.txt!
    component.createOperations();
	//创建开始菜单
    if (systemInfo.productType === "windows") {
		// "iconPath=%SystemRoot%/system32/SHELL32.dll","iconId=2"
        component.addOperation("CreateShortcut", "@TargetDir@/Lyrics.exe", "@StartMenuDir@/Spotify Lyrics.lnk",
            "workingDirectory=@TargetDir@", "description=Open Lyrics");
    }
	//创建桌面快捷方式
	try {
			// call the base create operations function
			component.createOperations();
			if (installer.value("os") == "win") { 
				try {
					var userProfile = installer.environmentVariable("USERPROFILE");
					installer.setValue("UserProfile", userProfile);
					component.addOperation("CreateShortcut", "@TargetDir@\\Lyrics.exe", "@UserProfile@\\Desktop\\Spotify Lyrics.lnk");
				} catch (e) {
					// Do nothing if key doesn't exist
				}
			}
		} catch (e) {
			print(e);
		}
}

Component.prototype.loaded = function ()
{
    var page = gui.pageByObjectName("DynamicPage");
    if (page != null) {
        console.log("Connecting the dynamic page entered signal.");
        page.entered.connect(Component.prototype.dynamicPageEntered);
    }
}

Component.prototype.dynamicPageEntered = function ()
{
    var pageWidget = gui.pageWidgetByObjectName("DynamicPage");
    if (pageWidget != null) {
        console.log("Setting the widgets label text.")
        pageWidget.m_pageLabel.text = "This is a dynamically created page.";
    }
}
