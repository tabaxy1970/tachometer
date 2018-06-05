;****************  SRAM define  ********************
		cblock	H'34'
div1d				;割られる数上位　（終了時には０になる）
div1c				; ～
div1b				; ～
div1a				;下位
div2d				;割る数上位       (変化せず戻る）
div2c				; ～
div2b				; ～
div2a				;下位
div3d				;答え上位　　　　（答えが返る）
div3c				; ～
div3b				; ～
div3a				;下位
div4d				;余り上位　　　　（余りが返る、内部ワークにも使用）
div4c				; ～
div4b				; ～
div4a				;余り下位
divl1				;内部ループ用
divl2				;内部ループ用
diverr				;割る数が０であった場合に１をセットして戻る

prm1b				; 変換したい値（１６ビット。変換後は壊れる）
prm1a				;～下位
prm3e				;10進格納場所（５バイト）
prm3d		
prm3c		
prm3b		
prm3a				;～下位
srlc1				; カウンタ（サブルーチンで使用）
srwk1				; 一時余り格納場所（サブルーチンで使用）

		endc


		org		H'100'


;		movlw	0ffh		;割られる数上位  (ffffffffh=4294967295)
;		movwf	div1d
;		movlw	0ffh		; ～
;		movwf	div1c
;		movlw	0ffh		; ～
;		movwf	div1b
;		movlw	0ffh		;下位
;		movwf	div1a

;		movlw	00h		;割る数上位       (989680h=10000000)
;		movwf	div2d
;		movlw	098h		; ～
;		movwf	div2c
;		movlw	096h 		; ～
;		movwf	div2b
;		movlw	080h		;下位
;		movwf	div2a

;		call	div32		;答えは div3d,c,b,a 、余りは div4d,c,b,a に返る

;*********************************************************************
;---------------------------------------------------------------------

div32		;３２÷３２ビット＝３２ビットの割り算ルーチン
		;div1d,c,b,a ÷ div2d,c,b,a ＝ 結果 div3d,c,b,a  余り div4d,c,b,a
		;演算後、式の左項（割られる数）は壊れる
		;割る数が０（エラー）なら diverr に１を代入して戻る
		;エラーの場合、引き数は変化しない
		;正常終了の場合は diverr=0 で戻る
		;32ビットで扱える数の最大は 4294967295　(42億9496万7295)
		movlw	H'20'		;20h=32
		movwf	divl1
		movf	div2a,W
		movwf	div4a		;割る数をワークにコピー
		movf	div2b,W
		movwf	div4b
		movf	div2c,W
		movwf	div4c
		movf	div2d,W
		movwf	div4d
di3201
		rlf		div4a,F		;左シフトする
		rlf		div4b,F
		rlf		div4c,F
		rlf		div4d,F
		btfsc	STATUS,W		;割る数の上位ビットが見付かったなら di3202 へ
		goto	di3202
		decfsz	divl1,F
		goto	di3201
		movlw	H'01'
		movwf	diverr		;割る数が０である、エラー終了
		return
di3202:
		clrf	div3a		;答えのクリア
		clrf	div3b
		clrf	div3c
		clrf	div3d
		clrf	div4a		;ワークのクリア
		clrf	div4b
		clrf	div4c
		clrf	div4d
		movlw	H'20'		;20h=32
		movwf	divl2
		movf	divl1,W
		subwf	divl2,F		;残り、実ループの回数
di3203
		bcf		STATUS,C		;キャリフラグを０に
		rlf		div1a,F		;有効位置までシフトする
		rlf		div1b,F
		rlf		div1c,F
		rlf		div1d,F
		rlf		div4a,F		;押し出されたビットをワークに
		rlf		div4b,F
		rlf		div4c,F
		rlf		div4d,F
		decfsz	divl1,F		;割られる数を初期位置までシフトしておく
		goto	di3203
di3204
		movf	div2d,W		;div2c を w にコピー
		subwf	div4d,W		;比較
		btfsc	STATUS,Z		;結果が＝なら di3205 へ
		goto	di3205
		btfsc	STATUS,C		;引くことが可なら di3210 へ
		goto	di3210
		btfss	STATUS,C		;引くことは不可なら di3211 へ
		goto	di3211
di3205:
		movf	div2c,W
		subwf	div4c,W
		btfsc	STATUS,Z
		goto	di3206
		btfsc	STATUS,C
		goto	di3210
		btfss	STATUS,C
		goto	di3211
di3206:
		movf	div2b,W
		subwf	div4b,W
		btfsc	STATUS,Z
		goto	di3207
		btfsc	STATUS,C
		goto	di3210
		btfss	STATUS,C
		goto	di3211
di3207:
		movf	div2a,W
		subwf	div4a,W
		btfsc	STATUS,Z
		goto	di3210
		btfss	STATUS,C
		goto	di3211
di3210:
		movf	div2a,W
		subwf	div4a,F		;ワークから下位を引く
		movlw	H'01'		;ワークには余りが残る
		btfss	STATUS,C
		subwf	div4b,F
		btfss	STATUS,C
		subwf	div4c,F
		btfss	STATUS,C
		subwf	div4d,F
		movf	div2b,W
		subwf	div4b,F		;ワークから２位を引く
		movlw	H'01'
		btfss	STATUS,C
		subwf	div4c,F
		btfss	STATUS,C
		subwf	div4d,F
		movf	div2c,W
		subwf	div4c,F		;ワークから３位を引く
		btfss	STATUS,W
		decf	div4d,F		;４上位 -1
		movf	div2d,W
		subwf	div4d,F		;ワークから上位を引く
		bsf		STATUS,C		;キャリフラグを１に
		goto	di3212
di3211
		bcf		STATUS,C		;キャリフラグを０に
di3212
		rlf		div3a,F		;キャリフラグの内容を答えにシフトしてゆく
		rlf		div3b,F
		rlf		div3c,F
		rlf		div3d,F
		movf	divl2,F		;divl2 が 0 か検査
		btfsc	STATUS,Z
		goto	di3213		;最下位まで処理したなら終了
		decf	divl2,F		;ビット位置を１つ下げる（右へ）
		bcf		STATUS,C		;キャリフラグを０に
		rlf		div1a,F		;ワークへ１ビット左シフト
		rlf		div1b,F
		rlf		div1c,F
		rlf		div1d,F
		rlf		div4a,F
		rlf		div4b,F
		rlf		div4c,F
		rlf		div4d,F
		goto	di3204
di3213
		clrf	diverr		;正常終了
		return


;*********************************************************************
;---------------------------------------------------------------------

		;結果はＢＣＤ数値（各下位４ビット）


hexdec16	;１６ビット（２バイト）数値を１０進数５桁のＢＣＤ値に変換する
		;引き数は prm1b(上位),prm1a(下位) の２バイト
		;結果は prm3e,prm3d,prm3c,prm3b,prm3a に得られ、右の prm3a が下位の数になる。

		movlw	srlc1		;格納場所初期値
		movwf	4h		;4h=fsr
		call	devide		;最下位変換
		call	devide
		call	devide
		call	devide
		call	devide		;最上位変換

		return

devide					;÷１０サブルーチン　（１０で除算）
		movlw	d'16'		;１６ビットくり返し
		movwf	srlc1
		clrf	srwk1
devide0
		bcf	3h,0		;キャリフラグのクリア
		rlf	prm1a,1
		rlf	prm1b,1
		rlf	srwk1,1
		movlw	b'11110110'
		addwf	srwk1,0
		btfsc	3h,0
		movwf	srwk1
		btfsc	3h,0
		incf	prm1a,1
		decfsz	srlc1,1
		goto	devide0
		decf	4h,1		;4h=fsr
		movf	srwk1,0
		movwf	0h		;余り (0h=indirect)

		return

;---------------------------------------------------------------------
;*********************************************************************

;---------------------------------------------------------------------
;*********************************************************************
