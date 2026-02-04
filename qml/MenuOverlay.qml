import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import GameEnums 1.0
import "Components"

Item {
    id: menuItem

    width: 200
    height: 1.5 * menuItem.width

    visible: GameController && GameController.gameState !== GameStates.RUNNING

    FontLoader {
        id: menuFontLoader
        source : "Assets/Fonts/PressStart2P-Regular.ttf"
    }

    Component.onCompleted: console.log(GameController.gameState)


    ColumnLayout {
        anchors.centerIn: parent

        MainMenuButton {
            id: playButton
            Layout.alignment: Qt.AlignHCenter

            labelText: "Play"
            visible: GameController && GameController.gameState === GameStates.STARTING
            onActionTriggered: function() {
                GameController.startGame()
            }
        }

        MainMenuButton {
            id: resumeButton

            Layout.alignment: Qt.AlignHCenter

            labelText: "Resume"
            visible: GameController && GameController.gameState === GameStates.PAUSED
            onActionTriggered: function() {
                GameController.resumeGame()
            }
        }

        MainMenuButton {
            id: restartButton

            Layout.alignment: Qt.AlignHCenter

            labelText: "Restart"
            visible: GameController && GameController.gameState === GameStates.PAUSED
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
