import QtQuick

Item {
    id: playerRect
    width: PlayerController === null ? 0 : PlayerController.playerWidth
    height: PlayerController === null ? 0 : PlayerController.playerHeight
    x: PlayerController === null ? 0 : PlayerController.playerCurrentX
    y: PlayerController === null ? 0 : PlayerController.playerCurrentY

    Image {
        id: playerImage
        anchors.fill: playerRect
        fillMode: Image.PreserveAspectFit
        source: "qrc:/qml/Assets/Player/Skyfire.png"
    }

    AnimatedImage {
        id: fireGif
        source: "qrc:/qml/Assets/Player/flame_flicker.gif"
        sourceSize: "30x30"
        x: playerImage.x + 37
        y: playerImage.y - 10
    }

    Keys.onPressed: (event) => {
                        if(event.key === Qt.Key_Left){
                            GameController.moveLeftPressed()
                        }
                        if(event.key === Qt.Key_Right){
                            GameController.moveRightPressed()
                        }
                        if(event.key === Qt.Key_Up){
                            // GameController.applyBoost()
                            GameController.setPressed(true)
                        }
                        if(event.key === Qt.Key_Space) {
                            GameController.shootBullet()
                        }
                        if(event.key === Qt.Key_Escape) {
                            GameController.togglePause()
                        }
                    }

    Keys.onReleased: (event) => {
                         if (event.key === Qt.Key_Left || event.key === Qt.Key_Right)
                            GameController.moveReleased()

                         if(event.key === Qt.Key_Up){
                             // GameController.applyBoost()
                             GameController.setPressed(false)
                         }
                     }


}
