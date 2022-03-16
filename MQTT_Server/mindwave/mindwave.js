const mqtt = require('mqtt');
const host = 'tailor.cloudmqtt.com';
const port = '18184'
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`
const connectUrl = `mqtt://${host}:${port}`
let client = mqtt.connect(connectUrl, {clientId, username: "kvuwrinm", password: "tucVHt31q7Gx", protocol: "mqtt"});

let attentionData = [49, 44, 36, 33, 38, 42, 45, 46, 49, 48, 54, 52, 57, 56, 50, 51, 46, 42, 41, 37, 40, 38, 35, 33, 33, 28, 24, 21, 25, 21, 15, 17, 13, 14, 13, 16, 16, 18, 100]
let meditationData = [53, 60, 73, 89, 75, 94, 73, 69, 62, 50, 42, 31, 36, 22, 46, 68, 75, 73, 83, 94, 81, 78, 56, 33, 37, 34, 29, 37, 42, 56, 62, 72, 55, 69, 79, 80, 75, 67, 51, 45, 32, 44, 25, 37, 0]
let theta = [60, 74, 95, 80, 54, 60, 69, 35, 50, 52, 74, 40, 22, 46, 85, 61, 49, 68, 58, 56, 88, 79, 66, 92, 83, 92, 42, 64, 93, 43]
let gamma = [74, 54, 41, 23, 86, 64, 44, 95, 51, 76, 43, 72, 51, 40, 13, 45, 58, 16, 11, 32, 61, 22, 54, 45, 39, 87, 42, 57, 64, 76]
let alpha = [10, 35, 12, 20, 31, 38, 34, 46, 64, 59, 46, 53, 24, 10, 22, 60, 56, 28, 14, 35, 68, 84, 98, 78, 100, 55, 18, 30, 19, 29]
let beta = [19, 40, 61, 32, 16, 41, 75, 40, 27, 26, 59, 77, 93, 83, 94, 56, 79, 57, 62, 83, 90, 66, 23, 33, 35, 69, 54, 87, 48, 51]

const delay = ms => new Promise(resolve => setTimeout(resolve, ms))


client.on('connect', function () {

    // attention data
    attention()
    // meditation()
    // vibration()

})

const attention = async () => {
    for (let num of attentionData) {
        client.publish('attention', num.toString())
        await delay(1000)
    }
}

const meditation = async () => {
    for (let num of meditationData) {
        client.publish('meditation', num.toString())
        await delay(1000)
    }
}

const vibration = async () => {
    let i = 99999
    while (i--) {
        for (let i = 0;i<30;i++){
            client.publish('theta', theta[i].toString())
            client.publish('gamma', gamma[i].toString())
            client.publish('alpha', alpha[i].toString())
            client.publish('beta', beta[i].toString())
            await delay(10)
        }

    }
}

client.on('message', function (topic, message) {
    // message is Buffer
    // client.subscribe('presence', function (err) {
    //     if (!err) {
    //         client.publish('presence', 'Hello mqtt')
    //     }
    // })
    console.log(message.toString())
})

