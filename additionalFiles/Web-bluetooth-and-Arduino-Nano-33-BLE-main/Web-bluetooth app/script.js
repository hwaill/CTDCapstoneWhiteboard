let x, y, z;
let ledState = false;

let xCharacteristic, yCharacteristic, zCharacteristic, ledCharacteristic;

const serviceUUID = "722cf000-6c3d-48ac-8180-64551d967680";

const toDoCharacteristicUUID = "722cf001-6c3d-48ac-8180-64551d967680";
const ledCharacteristicUUID = "722cf002-6c3d-48ac-8180-64551d967680";

let decoder = new TextDecoder("utf-8");
let encoder = new TextEncoder('utf-8');

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
    let sendMsg = encoder.encode("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis,.");
    
    await toDoCharacteristic.writeValueWithResponse(sendMsg);
    
    //any time you read a value
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