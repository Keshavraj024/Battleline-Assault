import QtQuick

Item {
    id: playerRect
    width: GameController === null ? 0 : GameController.playerWidth
    height: GameController === null ? 0 : GameController.playerHeight
    x: GameController === null ? 0 : GameController.currentX
    y: GameController === null ? 0 : GameController.currentY
    focus: true

    Image {
        id: playerImage
        anchors.fill: playerRect
        fillMode: Image.PreserveAspectFit
        source: "Assets/Player/Skyfire.png"
    }

    AnimatedImage {
        id: fireGif
        source: "Assets/Player/flame_flicker.gif"
        sourceSize: "30x30"
        x: playerImage.x + 37
        y: playerImage.y - 10
    }

    Keys.onPressed: (event) => {
                        if(event.key === Qt.Key_Left){
                            GameController.moveLeft()
                        }
                        if(event.key === Qt.Key_Right){
                            GameController.moveRight()
                        }
                        if(event.key === Qt.Key_Up){
                            GameController.applyBoost()
                        }
                        if(event.key === Qt.Key_Space) {
                            GameController.shootBullet()
                        }
                    }

    Keys.onReleased: (event) => {
                         if(event.key === Qt.Key_Left || event.key === Qt.Key_Right)
                         GameController.stopPlayerMoveTimer()
                     }
}
