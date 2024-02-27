let x, y, z;
let ledState = false;

let xCharacteristic, yCharacteristic, zCharacteristic, ledCharacteristic;

const serviceUUID = "19b10000-e8f2-537e-4f6c-d104768a1214";

const toDoCharacteristicUUID = "19b10001-e8f2-537e-4f6c-d104768a1214";
const ledCharacteristicUUID = "19b10004-e8f2-537e-4f6c-d104768a1214";

let decoder = new TextDecoder("utf-8");

async function connect(){

    const device = await navigator.bluetooth.requestDevice({filters: [{services: [serviceUUID]}]});
    const server = await device.gatt.connect();
    const service = await server.getPrimaryService(serviceUUID);

    toDoCharacteristic = await service.getCharacteristic(toDoCharacteristicUUID);
    toDoCharacteristic.readValue().then((result) => {
        document.getElementById('x').textContent = decoder.decode(result);
    });
    
    ledCharacteristic = await service.getCharacteristic(ledCharacteristicUUID);

    toDoCharacteristic.addEventListener('characteristicvaluechanged', readToDo);

}

function readToDo(event) {
    console.log("test");
    x = event.target.value;
    document.getElementById('x').textContent = decoder.decode(x);
}

async function toggleLED(){
    let encoder = new TextEncoder('utf-8');
    let sendMsg = encoder.encode("1000100010");

    await toDoCharacteristic.writeValue(sendMsg);
    toDoCharacteristic.readValue().then((result) => {
        document.getElementById('x').textContent = decoder.decode(result);
    });
    
    // ledState = !ledState;

    // let buffer = new ArrayBuffer(1);
    // let view = new Uint8Array(buffer);
    // view[0] = ledState;

    // await ledCharacteristic.writeValue(view);
}

document.getElementById('connect').addEventListener("click", connect);
document.getElementById('led').addEventListener("click", toggleLED);