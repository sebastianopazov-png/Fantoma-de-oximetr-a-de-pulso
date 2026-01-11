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
