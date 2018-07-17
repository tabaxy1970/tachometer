;********************************************************
; RPM Counter
;********************************************************

        list            p=pic16f84a
        include         p16f84a.inc
        __config _HS_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
        errorlevel      -302    ;Eliminate bank warning

		include			macro.inc




;****************  Label Definition  ********************

;****************  SRAM define  ********************
		cblock		h'0c'
IntSaveWreg
IntSaveStatus
IntSavePclath
IntSaveFsr

Timer1
Timer2
Timer3
Timer4

OldTimer1

Scan1
Scan2
Scan3
Scan4

OldRpm1
OldRpm2

IntRb0
IntSkip
LedPort
Led1
Led2
Led3
Led4
Led5


		endc
;****************  Program Start  ***********************
        org     0               ;Reset Vector
        goto    PowerOnReset
        org     4               ;Interrupt Vector
;*************** Interrupt*************
InterruptProc
		movwf	IntSaveWreg
		swapf	STATUS,W
		movwf	IntSaveStatus
		movf	PCLATH,W
		movwf	IntSavePclath

; Check Interrupt Flag
		btfsc	INTCON,T0IF
		call	TimerInterrupt
		btfsc	INTCON,INTF
		call	Rb0Interrupt

		clrwdt						;Clear watch dog timer and Prescaler
		movf	IntSavePclath,W
		movwf	PCLATH
		swapf	IntSaveStatus,W
		movwf	STATUS
		swapf	IntSaveWreg,F
		swapf	IntSaveWreg,W
		retfie
;*************** TIMER INT Proc ******************
TimerInterrupt
		call	LedDrive

; Timer incliment (24Bit+TimerA)
		incfsz	Timer2,F
		goto	TimerAddEnd
		incfsz	Timer3,F
		goto	TimerAddEnd
		incf	Timer4,F
TimerAddEnd

		bcf		INTCON,T0IF
		return
;*************** LED Drive Proc ********************
LedDrive
		decfsz	IntSkip,F
		return

		call	GetLedPortA
		movwf	PORTA
		movf	FSR,W
		movwf	IntSaveFsr
		movlw	Low Led1
		addwf	LedPort,W
		movwf	FSR
		movf	INDF,W
		call	GetLedPortB
		movwf	PORTB
		movf	IntSaveFsr
		movwf	FSR

		incf	LedPort,F
		movlw	H'05'
		subwf	LedPort,W
		btfsc	STATUS,Z
		clrf	LedPort

		movlw	H'0c'
		movwf	IntSkip
		return

GetLedPortA
		movf	LedPort,W
		andlw	H'07'
		addwf	PCL,F
		retlw	H'01'
		retlw	H'02'
		retlw	H'04'
		retlw	H'08'
		retlw	H'10'
		retlw	H'00'
		retlw	H'00'
		retlw	H'00'

GetLedPortB
		andlw	H'0f'
		addwf	PCL,F
		retlw	B'10111110'
		retlw	B'00110000'
		retlw	B'11101100'
		retlw	B'11111000'
		retlw	B'01110010'
		retlw	B'11011010'
		retlw	B'11011110'
		retlw	B'10110010'
		retlw	B'11111110'
		retlw	B'11111010'
		retlw	B'11110110'
		retlw	B'01011110'
		retlw	B'10001110'
		retlw	B'01111100'
		retlw	B'11001110'
		retlw	B'11000110'

;*************** RB0 INT Proc ********************
Rb0Interrupt
		btfss	INTCON,INTE			; RB0 Interrupt Enable check
		return

		movf	TMR0,W
		addlw	H'fc'				; Delay Read Count Gap Adjust 4 Count
		movwf	Timer1
		movf	Timer2,W
		movwf	Scan2
		movf	Timer3,W
		movwf	Scan3
		movf	Timer4,W
		movwf	Scan4

		clrf	Timer2
		clrf	Timer3
		clrf	Timer4

		movf	OldTimer1,W			; Lead Timer Adjustment
		subwf	Timer1,W
		movwf	Scan1
		movlw	H'01'
		btfss	STATUS,C
		subwf	Scan2,F
		btfss	STATUS,C
		subwf	Scan3,F
		btfss	STATUS,C
		subwf	Scan4,F

		movf	Timer1,W
		movwf	OldTimer1

		bsf		IntRb0,0			; RB0 Interrupt Flag
		bcf		INTCON,INTF			; Clear RB Int
		bsf		INTCON,INTE			; RB0 Interrupt Enable
		return
;****************  Initial Process  *********************
PowerOnReset
	    bsf		STATUS,RP0			;Change to Bank1 
		clrwdt						;Clear watch dog timer and Prescaler
		movlw	H'00'
        movwf	TRISA				;Set PORTA All Output
		movlw	H'01'
        movwf	TRISB				;Set PORTB to Output/Input(Bit0) mode
		movlw	H'01'
		movwf	OPTION_REG			;RB Input Pull Up & PreScale 1/4
        bcf		STATUS,RP0			;Change to Bank0

		movlw	H'00'
		movwf	TMR0				; TIMER0 Count Base Value
		bsf		INTCON,INTE			; RB0 Interrupt Enable
		bsf		INTCON,T0IE			; TIMER0 Interrupt Enable
		bsf		INTCON,GIE			; General Interrupt Enable

		clrf	Timer2
		clrf	Timer3
		clrf	Timer4
		movlw	H'0c'
		movwf	IntSkip

		clrf	Scan1
		clrf	Scan2
		clrf	Scan3
		clrf	Scan4

		clrf	OldRpm1
		clrf	OldRpm2

		clrf	OldTimer1
		clrf	IntRb0
MainLoop
		btfss	IntRb0,0
		goto	MainLoop
		clrf	IntRb0

		movlw	H'04'		; CPU clock(5MHz) prescale 1/4 * 60 (047868c0h=75000000)
		movwf	div1d
		movlw	H'78'		; ～
		movwf	div1c
		movlw	H'68'		; ～
		movwf	div1b
		movlw	H'c0'		; LSB
		movwf	div1a
		movf	Scan4,W		; divider MSB       (Scan1-4)
		movwf	div2d
		movf	Scan3,W		; ～
		movwf	div2c
		movf	Scan2,W		; ～
		movwf	div2b
		movf	Scan1,W		; LSB
		movwf	div2a
		call	div32		; Anser -> div3d,c,b,a 、surplus div4d,c,b,a return

		movf	div3c,W
		btfss	STATUS,Z
		goto	MainLoop	; Chataring Cut ( over 65535RPM )

		movf	OldRpm2,W
		subwf	div3b,W
		btfss	STATUS,C
		goto	SkipChataCheck
;		sublw	H'20'
;		btfss	STATUS,C
;		goto	MainLoop				; spinup 511RPM over
SkipChataCheck
		movf	div3b,W
		movwf	OldRpm2
		movf	div3a,W
		movwf	OldRpm1

		movf	div3b,W
		movwf	prm1b
		movf	div3a,W
		movwf	prm1a
		call	hexdec16

		movf	prm3e,W
		movwf	Led1
		movf	prm3d,W
		movwf	Led2
		movf	prm3c,W
		movwf	Led3
		movf	prm3b,W
		movwf	Led4
		movf	prm3a,W
		movwf	Led5

		goto	MainLoop


		include			math.inc

        end


