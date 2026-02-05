import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls.Material

import GameEnums 1.0
import "Components"

Window {
    id: root
    width: GameController?.windowWidth ?? 0
    height: GameController?.windowHeight ?? 0
    visible: true
    title: qsTr("Battleline Assault")

    property int score : GameController?.score ?? 0
    property int highestScore : GameController?.highestScore ?? 0
    property int gameLevel: GameController?.level ?? 0

    property var bullets: GameController?.bullets ?? 0
    property var enemies: GameController?.enemies ?? 0

    property bool isGameRunning: (GameController?.gameState === GameStates.RUNNING) ?? false
    property bool isGameOver: (GameController?.gameState === GameStates.GAMEOVER) ?? false

    Image {
        id: backgroundImage
        anchors.fill: parent
        // fillMode: Image.PreserveAspectFit
        source : "Assets/bgImg_1.png"
        Rectangle {
            id: backgroundRect
            anchors.fill: backgroundImage
            color: "black"
            opacity: 0.5
        }
    }

    MenuOverlay {
        id: menuOverlay
        anchors.centerIn: parent
    }

    Rectangle {
        id: gameOverRect
        anchors.fill: parent
        color: "black"
        opacity: 0.8
        visible: root.isGameOver

        FontLoader {
            id: fontLoader
            source : "Assets/Fonts/Doto-ExtraBold.ttf"
        }


        ColumnLayout {
            id: gameOverLayout
            width: 500
            height: 200
            spacing: 20
            anchors.centerIn: gameOverRect

            Text {
                id: gameOverText
                text: "Game Over!!!!"
                color: "white"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                font.family: fontLoader.font.family
                font.weight: fontLoader.font.weight
                font.styleName: fontLoader.font.styleName
                font.pixelSize: 24
            }

            Text {
                id: scoreText
                text: "Your Score " + root.score
                color: "white"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                font.family: fontLoader.font.family
                font.weight: fontLoader.font.weight
                font.styleName: fontLoader.font.styleName
                font.pixelSize: 24
            }

            RowLayout {
                spacing: 20
                Layout.preferredHeight: 20
                Layout.preferredWidth: gameOverLayout.width
                Button {
                    id: closeButton
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1

                    text: "Quit"

                    font.family: fontLoader.font.family
                    font.weight: fontLoader.font.weight
                    font.styleName: fontLoader.font.styleName
                    font.pixelSize: 24

                    onClicked: root.close()
                }
                Button {
                    id: restartButton
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1

                    text: "Main Menu"

                    font.family: fontLoader.font.family
                    font.weight: fontLoader.font.weight
                    font.styleName: fontLoader.font.styleName
                    font.pixelSize: 24

                    onClicked: {
                        GameController.gameState = GameStates.STARTING
                    }
                }
            }
        }
    }

    GameHUD {
        anchors {
            left: parent.left
            top: parent.top
            margins: 10
        }
        score: root.score
        highestScore: root.highestScore
        gameLevel: root.gameLevel
        isGameRunning: root.isGameRunning
    }

    Player {
        id: playerRect
        focus : root.isGameRunning
        visible: root.isGameRunning
    }


    Repeater {
        model: root.bullets
        delegate: PlayerBullet {
        }
    }

    Repeater {
        model: root.enemies
        delegate: Enemy {
        }
    }
}
