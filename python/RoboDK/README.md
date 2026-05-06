# INSTRUCCIONES DE OPERACIÓN
Este archivo contiene la simulación interactiva del Gemelo Digital de nuestra línea de empaquetado. La estación esta disenada para funcionar de forma autónoma recibiendo órdenes desde el exterior mediante el protocolo MQTT.

## PASOS PARA ARRANCAR LA ESTACÓN
1. Asegurate de tener este proyecto (.rdk) abierto en RoboDK.
2. En el árbol del proyecto de la izquierda, ejecuta los siguientes tres scripts de Python para que funcionen simultaneamente:
   - **`MqttListener`**
   - **`Main_Produccion`**

## INICIAR LA PRODUCCIÓN
Una vez los scripts esten corriendo, la estación se quedara a la espera. Para iniciar la fabricación, conectate al broker **"broker.emqx.io"** y envia los siguientes mensajes JSON en orden al topic ***"PR2_1_3/linea1/esp"***:

* **`{"cajas_tipo_1": "2"}`**
* **`{"cajas_tipo_2": "3"}`**
* **`{"tipo": "EMPIEZA"}`**

(Puedes cambiar los numeros para probar distintos tamanos de pedido).

### PARADA DE EMERGENCIA (E-STOP)
Si necesitas abortar la producción instantaneamente y devolver los robots a su posicion de seguridad, envia el siguiente mensaje JSON al mismo topic:

**`{"aviso": "EMERGENCIA"}`**

---------------------------------------------------------

## FIN DEL PEDIDO
Tras finalizar se observará como los robots regresan a su posición de reposo y el sistema se parará automáticamente.
