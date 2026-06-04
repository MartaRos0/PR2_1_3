# INSTRUCCIONES DE OPERACIÓN
Este archivo contiene la simulación interactiva del Gemelo Digital de nuestra línea de empaquetado. Aunque el fin del proyecto sea que la simulación RoboDK trabaje junto a una ESP32-S3 y una web para recibir las diferentes acciones de encendido / pedidos, la estación esta disenada para funcionar de forma autónoma recibiendo órdenes desde el exterior mediante el protocolo MQTT.

## IMPORTANTE - Configuración de MQTT 
Si se quiere probar el funcionamiento sin necesidad de la ESP y la web, se ha adjuntado una imagen llamada ***"config_mqttx"*** (para simular la web) y ***"conf_mqttx_esp"*** (para simular la esp) mostrando como dejar la conexión en la aplicación **MQTTx**.
También se recomienda subscribirse a los siguientes topics para ver como trabaja el proceso por dentro al completo:
   - **`PR2_1_3/linea1/esp`**
   - **`PR2_1_3/linea1/python`**
   - **`PR2_1_3/linea1/web`**

## PASOS PARA ARRANCAR LA ESTACIÓN
1. Asegurate de tener este proyecto (.rdk) abierto en RoboDK.
2. En el árbol del proyecto de la izquierda, ejecuta el siguiente scripts de Python:
   - **`Main_Produccion`**

## INICIAR LA PRODUCCIÓN
Una vez los scripts esten corriendo, la estación se quedara a la espera. Para iniciar la fabricación, conectate al broker **"broker.emqx.io"** y envia los siguientes mensajes JSON en orden al topic 
***"PR2_1_3/linea1/web"***:

* **`{"cajas_tipo_1": "2"}`**
* **`{"cajas_tipo_2": "3"}`**

***"PR2_1_3/linea1/esp"***:

* **`{"tipo": "EMPIEZA"}`**

(Puedes cambiar los numeros para probar distintos tamanos de pedido).

### PARADA DE EMERGENCIA (E-STOP)
Si necesitas abortar la producción instantaneamente y devolver los robots a su posicion de seguridad, envia el siguiente mensaje JSON al mismo topic que el mensaje EMPIEZA:

**`{"aviso": "EMERGENCIA"}`**

Si volvemos a indicar con el mensaje EMPIEZA la simulación proseguirá la producción con un avance de cinta (esto hará que si un proceso se ha quedado a medias no se termine, tomando ese pedido como "descartado").

---------------------------------------------------------

## FIN DEL PEDIDO
Tras finalizar se observará como los robots regresan a su posición de reposo y el sistema quedará a la espera de nuevos pedidos para procesar.
