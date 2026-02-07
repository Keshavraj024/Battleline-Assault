import QtQuick

Item {
    id: enemy

    width: enemyObject.enemyWidth
    height: enemyObject.enemyHeight
    x: enemyObject.enemyX
    y: enemyObject.enemyY

    Image {
        id: enemyImage
        anchors.fill: enemy
        fillMode: Image.PreserveAspectFit
        source: enemyObject.imageSource
    }
}
