import QtQuick

Item {
    id: playerBullet
    width: model.bulletWidth
    height: model.bulletHeight
    x: model.bulletX
    y: model.bulletY

    AnimatedImage {
        id: fireGif
        anchors.fill: parent
        source: "Assets/Player/flame_flicker.gif"
        sourceSize: "50x30"
    }
}
