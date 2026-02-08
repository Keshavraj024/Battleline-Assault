import QtQuick

Item {
    id: playerBullet
    width: bulletObject.bulletWidth
    height: bulletObject.bulletHeight
    x: bulletObject.bulletX
    y: bulletObject.bulletY

    AnimatedImage {
        id: fireGif
        anchors.fill: parent
        source: "qrc:/qml/Assets/Player/flame_flicker.gif"
        sourceSize: "50x30"
    }
}
