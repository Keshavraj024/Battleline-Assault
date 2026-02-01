import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls.Material

Window {
    id: root
    width: GameController === null ? 0 : GameController.windowWidth
    height: GameController === null ? 0 : GameController.windowHeight
    visible: true
    title: qsTr("Battleline Assault")

    property int score : GameController === null ? 0 : GameController.score
    property int highestScore : GameController === null ? 0 : GameController.highestScore
    property int gameLevel: GameController === null ? 0 : GameController.level

    FontLoader {
        id: fontLoader
        source : "Assets/Fonts/Doto-ExtraBold.ttf"
    }


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

    Rectangle {
        id: gameOverRect
        anchors.fill: parent
        color: "black"
        opacity: 0.8
        visible: false

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

                    text: "Close"

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

                    text: "Restart"

                    font.family: fontLoader.font.family
                    font.weight: fontLoader.font.weight
                    font.styleName: fontLoader.font.styleName
                    font.pixelSize: 24

                    onClicked: {
                        gameOverRect.visible = false
                        playerRect.focus = true
                        GameController.initialize()
                    }
                }
            }
        }

        Connections {
            target: GameController
            function onGameOver() {
                gameOverRect.visible = true
                playerRect.focus = false
            }
        }
    }

    Player {
        id: playerRect
    }

    CustomText {
        id: scoreBoard

        customText: "Score : " + root.score

        anchors {
            left: parent.left
            leftMargin: 5
            top: parent.top
            topMargin: 5
        }
    }

    CustomText {
        id: highestScoreBoard
        customText: "Highest Score : " + root.highestScore

        anchors {
            left: parent.left
            leftMargin: 5
            top: scoreBoard.bottom
            topMargin: 5
        }
    }

    CustomText {
        id: levelBoard
        customText: "Level : " + root.gameLevel

        anchors {
            left: parent.left
            leftMargin: 5
            top: highestScoreBoard.bottom
            topMargin: 10
        }
    }

    Repeater {
        model: GameController === null ? 0 : GameController.bullets
        delegate: PlayerBullet {
        }
    }

    Repeater {
        model: GameController === null ? 0 : GameController.enemies
        delegate: Enemy {
        }
    }
}
