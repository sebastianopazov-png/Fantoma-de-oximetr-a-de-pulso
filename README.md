# Fantoma-de-oximetria-de-pulso
Codigo tanto para Arduino UNO (Maestro), como ESP32 (Esclavo), que tienen como proposito controlar un fantoma de oximetria de pulso que funciona entre los 60 a 100 BPM y entre 90 a 100% de SpO2

El desarrollo del presente trabajo se abordó mediante una metodología de tipo aplicada y experimental, orientada al diseño, implementación y validación de un prototipo funcional de fantoma de dedo humano para la simulación de parámetros fisiológicos. Esta metodología combina etapas de desarrollo iterativo de hardware y software, junto con procesos de verificación y validación experimental, permitiendo evaluar el desempeño del sistema en condiciones controladas y comparables con escenarios clínicos reales.

En una primera etapa, se realizó una revisión del estado del arte, con el objetivo de analizar trabajos previos relacionados con fantomas de fotopletismografía y oximetría de pulso, identificando tanto sus principales logros como sus limitaciones técnicas. Este análisis permitió definir criterios de diseño y mejoras a implementar en la presente versión del fantoma, tales como la estabilidad de la señal PPG, la simulación de valores de SpO2 clínicamente relevantes y la incorporación de un sistema de comunicación serial para la configuración remota de parámetros.

Posteriormente, se desarrolló la arquitectura del sistema, considerando tanto el diseño electrónico como la estructura física del fantoma. El enfoque adoptado fue modular, separando el sistema en subsistemas claramente definidos: generación de señales fisiológicas (SpO2, frecuencia cardíaca y PPG), control mediante microcontrolador, interfaz de comunicación serial RS-485 y acoplamiento físico con la pinza de oximetría del monitor clínico. Esta modularidad facilita futuras modificaciones, mejoras o reemplazos de componentes específicos sin afectar el funcionamiento global del sistema.

En cuanto a la metodología de desarrollo, se utilizó un enfoque incremental, en el cual cada modificación del hardware o del código fue seguida de pruebas funcionales parciales. Este procedimiento permitió detectar tempranamente errores de diseño, problemas de estabilidad o inconsistencias en la medición, reduciendo el impacto de fallas en etapas avanzadas del proyecto. Las decisiones de ajuste, tanto en la intensidad de los diodos emisores como en la forma de la señal PPG, se realizaron de manera experimental, apoyadas en la observación directa de la respuesta del monitor clínico.

La validación experimental del prototipo se llevó a cabo mediante pruebas controladas utilizando un monitor multiparámetro clínico, asegurando condiciones reproducibles y comparables. Se empleó un simulador comercial como referencia inicial para verificar el correcto funcionamiento del equipo de medición, permitiendo atribuir las variaciones observadas exclusivamente al desempeño del fantoma desarrollado. Los datos obtenidos fueron posteriormente analizados mediante herramientas estadísticas, incluyendo análisis de dispersión y concordancia, con el fin de evaluar la estabilidad y precisión del sistema.

Los materiales utilizados fueron:

•	ESP32-WROOM-32D
•	DAC MCP4725
•	LED rojo (660 nm)
•	LED infrarrojo (940 nm)
•	Resistencias de 330 Ω
•	Protoboards y cables jumper
•	2 modulos MAX485
•	Fantoma de dedo creado en la versión 3.0
•	Monitor General Meditech G3H 
•	Pinza de SpO2 del monitor




IMPLEMENTACIÓN


Inicialmente se requiere de un espacio de trabajo adecuado, amplio con el fin de no mover el fantoma cuando se este haciendo la toma de muestras, de segunda no tiene que estar con luz artificial (luz incandecente, pantrallas de computadoras) para que esta no afecte en la medición, además de no estar cerca de ventanas donde ingrese de manera difecta la luz natural hacia el sensor.
Luego se requiere del monitor General Meditech G3H, en el cual se verifique con un simulador que el monitor se encuentra midiendo dentro de los parámetros indicados, en este caso se verifico con un simulador MS100.

Una vez con todo lo anterior presente, se tiene que introducir el fantoma en la pinza de medición, esta tiene dos partes, en la parte superir se encuentran los diodos emisores y en la interior se encuentra el fototransistor el cual permite detectar las intensidades de las luces rojas e IR. Al momento de colocar el fantoma, se tiene que colocar los diodos emisores del fantoma apuntando hacia el sensor de la pinza, es decir, coincidir la parte inferior del dedo (yema del dedo “fantoma”) con el sensor de la pinza. El fantoma de dedo en conjunto con la pinza tiene que quedar inmóvil en posición vertica, apoyado en la mesa, es decir como si uno se colocara la pinza y lo apoyara en la mesa.

Con la parte física ya completa, se tiene que ir a la computadora donde se encuentra conectado el fantoma. Esta se puede controlar de 2 maneras diferentes, la primera esta pensada utilizando la comunicación serial RS-485 que tiene integrado el fantoma, donde por medio de un Arduino UNO con los códigos “S100 , S90” y sus variaciones en el tema numérico para variar el SpO2 o “B100, B60” para variar la frecuencia cardiaca. Estos valores se tienen que colocar en el monitor serial de la aplicación de Arduino IDE. 

El segundo método que se puede hacer para su variación en los parámetros es mediante la modificación de manera directa en el código del fantoma, el cual se encuentra en el ESP32. En este código en las primeras líneas de código se encuentran los parámetros que uno quiere que el fantoma simule, siendo SpO2 y BPM, acá solo se tiene que cambiar los valores predeterminados por los que uno quiere simular y ver los datos en el monitor multiparámetro.
