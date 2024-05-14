PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//246495/1207301/2.50/10/3/Integrated Circuit

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r140_30"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.3) (shapeHeight 1.4))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "SOP50P490X110-10N" (originalName "SOP50P490X110-10N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r140_30) (pt -2.2, 1) (rotation 90))
			(pad (padNum 2) (padStyleRef r140_30) (pt -2.2, 0.5) (rotation 90))
			(pad (padNum 3) (padStyleRef r140_30) (pt -2.2, 0) (rotation 90))
			(pad (padNum 4) (padStyleRef r140_30) (pt -2.2, -0.5) (rotation 90))
			(pad (padNum 5) (padStyleRef r140_30) (pt -2.2, -1) (rotation 90))
			(pad (padNum 6) (padStyleRef r140_30) (pt 2.2, -1) (rotation 90))
			(pad (padNum 7) (padStyleRef r140_30) (pt 2.2, -0.5) (rotation 90))
			(pad (padNum 8) (padStyleRef r140_30) (pt 2.2, 0) (rotation 90))
			(pad (padNum 9) (padStyleRef r140_30) (pt 2.2, 0.5) (rotation 90))
			(pad (padNum 10) (padStyleRef r140_30) (pt 2.2, 1) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -3.15 1.8) (pt 3.15 1.8) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 3.15 1.8) (pt 3.15 -1.8) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 3.15 -1.8) (pt -3.15 -1.8) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -3.15 -1.8) (pt -3.15 1.8) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 1.5) (pt 1.5 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 1.5) (pt 1.5 -1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 -1.5) (pt -1.5 -1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 -1.5) (pt -1.5 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 1) (pt -1 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.15 1.5) (pt 1.15 1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 1.15 1.5) (pt 1.15 -1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 1.15 -1.5) (pt -1.15 -1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.15 -1.5) (pt -1.15 1.5) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -2.9 1.5) (pt -1.5 1.5) (width 0.2))
		)
	)
	(symbolDef "ADS1115IDGSR" (originalName "ADS1115IDGSR")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 3) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 4) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 5) (pt 0 mils -400 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -425 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 6) (pt 1400 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 7) (pt 1400 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 8) (pt 1400 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 9) (pt 1400 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 10) (pt 1400 mils -400 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -425 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(line (pt 200 mils 100 mils) (pt 1200 mils 100 mils) (width 6 mils))
		(line (pt 1200 mils 100 mils) (pt 1200 mils -500 mils) (width 6 mils))
		(line (pt 1200 mils -500 mils) (pt 200 mils -500 mils) (width 6 mils))
		(line (pt 200 mils -500 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 1250 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 1250 mils 200 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "ADS1115IDGSR" (originalName "ADS1115IDGSR") (compHeader (numPins 10) (numParts 1) (refDesPrefix IC)
		)
		(compPin "1" (pinName "ADDR") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "ALERT/RDY") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "3" (pinName "GND") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "4" (pinName "AIN0") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "5" (pinName "AIN1") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "10" (pinName "SCL") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "9" (pinName "SDA") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "8" (pinName "VDD") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "7" (pinName "AIN3") (partNum 1) (symPinNum 9) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "6" (pinName "AIN2") (partNum 1) (symPinNum 10) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "ADS1115IDGSR"))
		(attachedPattern (patternNum 1) (patternName "SOP50P490X110-10N")
			(numPads 10)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
				(padNum 9) (compPinRef "9")
				(padNum 10) (compPinRef "10")
			)
		)
		(attr "Mouser Part Number" "595-ADS1115IDGSR")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/ADS1115IDGSR?qs=IK5e5L0zOXjKCJoVMhYO%2FQ%3D%3D")
		(attr "Manufacturer_Name" "Texas Instruments")
		(attr "Manufacturer_Part_Number" "ADS1115IDGSR")
		(attr "Description" "16-Bit 860SPS 4-Ch Delta-Sigma ADC With PGA, Oscillator, Vref, Comparator, and I2C")
		(attr "<Hyperlink>" "http://www.ti.com/lit/gpn/ADS1115")
		(attr "<Component Height>" "1.1")
		(attr "<STEP Filename>" "ADS1115IDGSR.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0.69")
		(attr "<STEP Rotation>" "X=90;Y=0;Z=90")
	)

)
