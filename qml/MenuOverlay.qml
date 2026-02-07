import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import GameEnums 1.0
import "Components"
import "Customs"

Item {
    id: menuItem

    width: 200
    height: 1.5 * menuItem.width

    property bool isRunning: GameController?.gameState === GameStates.RUNNING ?? false
    property bool isGameOver: GameController?.gameState === GameStates.GAMEOVER ?? false
    property bool isStarting: GameController?.gameState === GameStates.STARTING ?? false
    property bool isPaused: GameController?.gameState === GameStates.PAUSED ?? false

    FontLoader {
        id: menuFontLoader
        source : "qrc:/qml/Assets/Fonts/PressStart2P-Regular.ttf"
    }

    Keys.onPressed: (event) => {
                        if(event.key === Qt.Key_Escape) {
                            GameController.togglePause()
                        }
                    }


    ColumnLayout {
        anchors.centerIn: parent

        CustomText {
            id: gameTitle
            customText: "BATTLELINE ASSAULT"
            fontName: menuFontLoader
            font.pixelSize: 35
            Layout.alignment: Qt.AlignHCenter
        }

        MainMenuButton {
            id: playButton
            Layout.alignment: Qt.AlignHCenter

            labelText: "Play"
            visible: isStarting
            onActionTriggered: function() {
                GameController.startGame()
            }
        }

        MainMenuButton {
            id: resumeButton

            Layout.alignment: Qt.AlignHCenter

            labelText: "Resume"
            visible: isPaused
            onActionTriggered: function() {
                GameController.togglePause()
            }
        }

        MainMenuButton {
            id: restartButton

            Layout.alignment: Qt.AlignHCenter

            labelText: "Restart"
            visible: isPaused
            onActionTriggered: function() {
                GameController.restartGame()

            }
        }

        MainMenuButton {
            id: quitButton

            Layout.alignment: Qt.AlignHCenter

            labelText: "Quit"
            onActionTriggered: function() {
                GameController.quitGame()
            }
        }
    }
}
