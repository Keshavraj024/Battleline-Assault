import QtQuick

Item {
    id: enemy

    property var enemyImageSource : [
        "Assets/Enemies/AbyssalHexcrawler.png",
        "Assets/Enemies/EmberclawRavager.png",
        "Assets/Enemies/IronbaneSkitterlord.png",
        "Assets/Enemies/NetherfangSentinel.png",
        "Assets/Enemies/PyroniteWarDrone.png",
        "Assets/Enemies/VoidreaverStrix.png",
        "Assets/Enemies/PyrothorSovereign.png",
        "Assets/Enemies/VoidclawIncinerator.png",
        "Assets/Enemies/AshmawSpectralite.png",
        "Assets/Enemies/BlazefangOverseer.png",
        "Assets/Enemies/InfernicTalonspawn.png",
        "Assets/Enemies/CindervoidRavager.png",
    ]

    width: model.enemyWidth
    height: model.enemyHeight
    x: model.enemyX
    y: model.enemyY

    Image {
        id: enemyImage
        anchors.fill: enemy
        fillMode: Image.PreserveAspectFit
        source: enemyImageSource[model.enemyImageIndex]
    }
}
