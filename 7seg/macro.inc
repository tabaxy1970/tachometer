
;	Pic macro

TempWreg	equ		h'4f'


pcall	macro	Addr
		movlw	high Addr
		movwf	PCLATH
		call	Addr
		movlw	high $
		movwf	PCLATH
		endm

pgoto	macro	Addr
		movlw	high Addr
		movwf	PCLATH
		goto	Addr
		endm

; Compre macro

cmpsz	macro	val
		movwf	TempWreg
		sublw	val
		swapf	TempWreg,F
		swapf	TempWreg,W
		btfss	STATUS,Z
		endm

cmpsnz	macro	val
		movwf	TempWreg
		sublw	val
		swapf	TempWreg,F
		swapf	TempWreg,W
		btfsc	STATUS,Z
		endm

cmpscc	macro	val
		movwf	TempWreg
		sublw	val
		swapf	TempWreg,F
		swapf	TempWreg,W
		btfsc	STATUS,C
		endm

cmpscs	macro	val
		movwf	TempWreg
		sublw	val
		swapf	TempWreg,F
		swapf	TempWreg,W
		btfss	STATUS,C
		endm

;		movlw	H'12'
;		cmpsz	H'12'	; Equal Skip
;		nop				; skip
;		cmpsnz	H'12'	; Not Eaqual Skip
;		nop				; exec
;		cmpscc	H'1'	; Wreg > k to Skip
;		nop				; skip
;		cmpscc	H'13' 
;		nop				; exec
;		cmpscs	H'1'	; Wreg < k to Skip
;		nop				; exec
;		cmpscs	H'13'
;		nop				; skip


add16ff	macro	addfile1,addfile2,distfile
		movf	addfile1,W
		addwf	addfile2,W
		movwf	distfile
		movf	addfile1+1,W
		btfsc	STATUS,C
		incf	addfile1+1,W
		addwf	addfile2+1,W
		movwf	distfile+1
		endm


; compfile1 smole equal big
cmp16ff	macro	compfile1,compfile2,AddrS,AddrE,AddrB
		movf	compfile1+1,W
		addlw	H'80'
		movwf	TempWreg
		movf	compfile2+1,W
		addlw	H'80'
		subwf	TempWreg,W
		btfsc	STATUS,Z
		goto	$+4
		btfsc	STATUS,C
		goto	AddrS
		goto	AddrB
		movf	compfile2,W
		subwf	compfile1,W
		btfsc	STATUS,Z
		goto	AddrE
		btfsc	STATUS,C
		goto	AddrS
		goto	AddrB
		endm


		
