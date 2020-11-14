# FM_transceiver
FM Transceiver mit dem DRA818V.
Diese Project beschreibt den Aufbau eines FM Transeiver für das 2 Meter Band mit dem SA818V oder DRA818V Modul.
Als Processor wird ein CY8c27443-PXI verwendet.
Der Code ist in c und Assembler geschrieben mit dem PSOC Designer 5.1

- Spannungsversorung 7 bis 14 Volt extern eingespeist über den  DC Jack 
  mit gleichzeitigem laden des LIPO AKKu 
- ohne externe Einspeisung erfolgt die Stromversorung über einen internen LIPO Akku
- internes oder  externes microfon
- externer PTT Schaltausgang
- USB Cat Schnittstelle 
- als Anzeige dient ein 4x20 Character LCD für 3,3 Volt 
- Bluetooth Option auf der Leiterplatte integriert

Der Stromverbrauch beträgt im Empfangsfall 110 mA.
Die Leistung 0,75 Watt im High Mode oder 0,25 Watt im Low Mode.
Der TRX kann ohne Display betrieben werden die Einstellungen sind dann über die Cat Schnittstelle 
vorzunehmen.
