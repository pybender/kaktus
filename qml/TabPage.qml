/*
  Copyright (C) 2014 Michal Kosciesza <michal@mkiol.net>

  This file is part of Kaktus.

  Kaktus is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kaktus is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kaktus.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: root

    ListViewWithBar {
        id: listView
        anchors.fill: parent
        model: tabModel

        onBarShowRequest: {
            controlbar.show();
        }

        onBarHideRequest: {
            controlbar.hide();
        }

        MainMenu{}
        //UpMenu {}

        header: PageHeader {
            title: "Tabs"
        }

        delegate: ListItem {
            id: listItem
            contentHeight: item.height + 2 * Theme.paddingMedium

            Column {
                id: item
                spacing: Theme.paddingSmall
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width

                Label {
                    id: label
                    wrapMode: Text.AlignLeft
                    anchors.left: parent.left; anchors.right: parent.right;
                    anchors.leftMargin: Theme.paddingLarge; anchors.rightMargin: Theme.paddingLarge
                    font.pixelSize: Theme.fontSizeMedium
                    text: title
                }
            }

            Image {
                id: image
                width: Theme.iconSizeSmall
                height: Theme.iconSizeSmall
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
                anchors.verticalCenter: item.verticalCenter
            }

            Component.onCompleted: image.source = cache.getUrlbyUrl(iconUrl)

            onClicked: {
                utils.setFeedModel(uid);
                pageStack.push(Qt.resolvedUrl("FeedPage.qml"),{"title": title});
                console.log(cache.getUrlbyUrl(iconUrl));
            }
        }


        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No tabs")
        }

        VerticalScrollDecorator {
            flickable: listView
        }

    }

    ControlBar {
        id: controlbar
    }

}
