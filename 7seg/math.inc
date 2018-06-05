;****************  SRAM define  ********************
		cblock	H'34'
div1d				;�����鐔��ʁ@�i�I�����ɂ͂O�ɂȂ�j
div1c				; �`
div1b				; �`
div1a				;����
div2d				;���鐔���       (�ω������߂�j
div2c				; �`
div2b				; �`
div2a				;����
div3d				;������ʁ@�@�@�@�i�������Ԃ�j
div3c				; �`
div3b				; �`
div3a				;����
div4d				;�]���ʁ@�@�@�@�i�]�肪�Ԃ�A�������[�N�ɂ��g�p�j
div4c				; �`
div4b				; �`
div4a				;�]�艺��
divl1				;�������[�v�p
divl2				;�������[�v�p
diverr				;���鐔���O�ł������ꍇ�ɂP���Z�b�g���Ė߂�

prm1b				; �ϊ��������l�i�P�U�r�b�g�B�ϊ���͉���j
prm1a				;�`����
prm3e				;10�i�i�[�ꏊ�i�T�o�C�g�j
prm3d		
prm3c		
prm3b		
prm3a				;�`����
srlc1				; �J�E���^�i�T�u���[�`���Ŏg�p�j
srwk1				; �ꎞ�]��i�[�ꏊ�i�T�u���[�`���Ŏg�p�j

		endc


		org		H'100'


;		movlw	0ffh		;�����鐔���  (ffffffffh=4294967295)
;		movwf	div1d
;		movlw	0ffh		; �`
;		movwf	div1c
;		movlw	0ffh		; �`
;		movwf	div1b
;		movlw	0ffh		;����
;		movwf	div1a

;		movlw	00h		;���鐔���       (989680h=10000000)
;		movwf	div2d
;		movlw	098h		; �`
;		movwf	div2c
;		movlw	096h 		; �`
;		movwf	div2b
;		movlw	080h		;����
;		movwf	div2a

;		call	div32		;������ div3d,c,b,a �A�]��� div4d,c,b,a �ɕԂ�

;*********************************************************************
;---------------------------------------------------------------------

div32		;�R�Q���R�Q�r�b�g���R�Q�r�b�g�̊���Z���[�`��
		;div1d,c,b,a �� div2d,c,b,a �� ���� div3d,c,b,a  �]�� div4d,c,b,a
		;���Z��A���̍����i�����鐔�j�͉���
		;���鐔���O�i�G���[�j�Ȃ� diverr �ɂP�������Ė߂�
		;�G���[�̏ꍇ�A�������͕ω����Ȃ�
		;����I���̏ꍇ�� diverr=0 �Ŗ߂�
		;32�r�b�g�ň����鐔�̍ő�� 4294967295�@(42��9496��7295)
		movlw	H'20'		;20h=32
		movwf	divl1
		movf	div2a,W
		movwf	div4a		;���鐔�����[�N�ɃR�s�[
		movf	div2b,W
		movwf	div4b
		movf	div2c,W
		movwf	div4c
		movf	div2d,W
		movwf	div4d
di3201
		rlf		div4a,F		;���V�t�g����
		rlf		div4b,F
		rlf		div4c,F
		rlf		div4d,F
		btfsc	STATUS,W		;���鐔�̏�ʃr�b�g�����t�������Ȃ� di3202 ��
		goto	di3202
		decfsz	divl1,F
		goto	di3201
		movlw	H'01'
		movwf	diverr		;���鐔���O�ł���A�G���[�I��
		return
di3202:
		clrf	div3a		;�����̃N���A
		clrf	div3b
		clrf	div3c
		clrf	div3d
		clrf	div4a		;���[�N�̃N���A
		clrf	div4b
		clrf	div4c
		clrf	div4d
		movlw	H'20'		;20h=32
		movwf	divl2
		movf	divl1,W
		subwf	divl2,F		;�c��A�����[�v�̉�
di3203
		bcf		STATUS,C		;�L�����t���O���O��
		rlf		div1a,F		;�L���ʒu�܂ŃV�t�g����
		rlf		div1b,F
		rlf		div1c,F
		rlf		div1d,F
		rlf		div4a,F		;�����o���ꂽ�r�b�g�����[�N��
		rlf		div4b,F
		rlf		div4c,F
		rlf		div4d,F
		decfsz	divl1,F		;�����鐔�������ʒu�܂ŃV�t�g���Ă���
		goto	di3203
di3204
		movf	div2d,W		;div2c �� w �ɃR�s�[
		subwf	div4d,W		;��r
		btfsc	STATUS,Z		;���ʂ����Ȃ� di3205 ��
		goto	di3205
		btfsc	STATUS,C		;�������Ƃ��Ȃ� di3210 ��
		goto	di3210
		btfss	STATUS,C		;�������Ƃ͕s�Ȃ� di3211 ��
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
		subwf	div4a,F		;���[�N���牺�ʂ�����
		movlw	H'01'		;���[�N�ɂ͗]�肪�c��
		btfss	STATUS,C
		subwf	div4b,F
		btfss	STATUS,C
		subwf	div4c,F
		btfss	STATUS,C
		subwf	div4d,F
		movf	div2b,W
		subwf	div4b,F		;���[�N����Q�ʂ�����
		movlw	H'01'
		btfss	STATUS,C
		subwf	div4c,F
		btfss	STATUS,C
		subwf	div4d,F
		movf	div2c,W
		subwf	div4c,F		;���[�N����R�ʂ�����
		btfss	STATUS,W
		decf	div4d,F		;�S��� -1
		movf	div2d,W
		subwf	div4d,F		;���[�N�����ʂ�����
		bsf		STATUS,C		;�L�����t���O���P��
		goto	di3212
di3211
		bcf		STATUS,C		;�L�����t���O���O��
di3212
		rlf		div3a,F		;�L�����t���O�̓��e�𓚂��ɃV�t�g���Ă䂭
		rlf		div3b,F
		rlf		div3c,F
		rlf		div3d,F
		movf	divl2,F		;divl2 �� 0 ������
		btfsc	STATUS,Z
		goto	di3213		;�ŉ��ʂ܂ŏ��������Ȃ�I��
		decf	divl2,F		;�r�b�g�ʒu���P������i�E�ցj
		bcf		STATUS,C		;�L�����t���O���O��
		rlf		div1a,F		;���[�N�ւP�r�b�g���V�t�g
		rlf		div1b,F
		rlf		div1c,F
		rlf		div1d,F
		rlf		div4a,F
		rlf		div4b,F
		rlf		div4c,F
		rlf		div4d,F
		goto	di3204
di3213
		clrf	diverr		;����I��
		return


;*********************************************************************
;---------------------------------------------------------------------

		;���ʂ͂a�b�c���l�i�e���ʂS�r�b�g�j


hexdec16	;�P�U�r�b�g�i�Q�o�C�g�j���l���P�O�i���T���̂a�b�c�l�ɕϊ�����
		;�������� prm1b(���),prm1a(����) �̂Q�o�C�g
		;���ʂ� prm3e,prm3d,prm3c,prm3b,prm3a �ɓ����A�E�� prm3a �����ʂ̐��ɂȂ�B

		movlw	srlc1		;�i�[�ꏊ�����l
		movwf	4h		;4h=fsr
		call	devide		;�ŉ��ʕϊ�
		call	devide
		call	devide
		call	devide
		call	devide		;�ŏ�ʕϊ�

		return

devide					;���P�O�T�u���[�`���@�i�P�O�ŏ��Z�j
		movlw	d'16'		;�P�U�r�b�g����Ԃ�
		movwf	srlc1
		clrf	srwk1
devide0
		bcf	3h,0		;�L�����t���O�̃N���A
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
		movwf	0h		;�]�� (0h=indirect)

		return

;---------------------------------------------------------------------
;*********************************************************************

;---------------------------------------------------------------------
;*********************************************************************