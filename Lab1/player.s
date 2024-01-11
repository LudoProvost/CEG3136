
  AREA    |.text|, CODE, READONLY
  PRESERVE8;
  THUMB;


  EXPORT update_points_s

update_points_s FUNCTION		; char update_points_s(player_t *player, uint8_t target_info);
	; r0 *player
	; r1 target_info
	PUSH          {R4-R7, LR}
	MOV           R2,R0
	MOVS R3, #0					; int points_urned = 0
	; player ->nAttempts++
	LDRH          R0,[R2,#0]	
	ADDS          R0,R0,#1
	STRH          R0,[R2,#0]
CONDITION_1						; if (target_info & 0xFC) >> mask bit 0 & 1, which are used to flage game state 
	AND           R0,R1,#0xFC
	CBZ           R0,POINT_UPDATE
CONDITION_2						; if ((target_info & 0x2) == 0) // Target hit
	AND           R0,R1,#2
	CBNZ          R0,POINT_UPDATE
	LDRH          R5,[R2,#2]	; player->nHits++  // Hit 
	ADDS          R5,R5,#1
	STRH          R5,[R2,#2]
	UBFX          R0,R1,#2,#3	; type = (target_info & 0x1C) >> 2
	UBFX          R4,R1,#5,#2	; indx = (target_info & 0x60) >> 5 
	SUBS          R5,R0,#1		; player->pieces_hit[type-1][indx]++
	ADDS          R6,R2,#6
	ADD           R5,R6,R5,LSL #2
	LDRB          R5,[R5,R4]
	ADDS          R5,R5,#1
	UXTB          R7,R5
	SUBS          R5,R0,#1
	ADD           R5,R6,R5,LSL #2
	STRB          R7,[R5,R4]
	ADDS          R3,R3,#1		; points_urned++
	SUBS          R5,R0,#1
	ADD           R5,R6,R5,LSL #2	;if (player->pieces_hit[type-1][indx] == type) // Piece Destroyed
	LDRB          R5,[R5,R4]
	CMP           R5,R0
	BNE           NO_OPERATION
	ADD           R3,R3,R0		; points_urned += type // Destroy points bonus
NO_OPERATION
	NOP
POINT_UPDATE
	LDRH          R0,[R2,#4]	; pplayer->points += points_urned
	ADD           R0,R0,R3
	STRH          R0,[R2,#4]
	MOV	R0, R3					; return points_urned
	POP           {R4-R7, PC}
	BX	LR		
  ENDFUNC
  

  EXPORT player_get_num_pieces_destroyed_s

player_get_num_pieces_destroyed_s FUNCTION	; int  player_get_num_pieces_destroyed_s (player_t * player);
	PUSH {R4-R9}
	
	ADD R0, R0, #6		; player->pieces_hit
	MOV R4, #0			; n = 0
	MOV R5, #0			; i =0
I_LOOP_1	 			; i loop
	ADD R9, R5, #1		; R9 <- i + 1
	LSL R7, R5, #2		; 
	ADD R7, R0, R7		; r7 <- pieces_hit[i]
	MOV R6, #0			; j =0
J_LOOP_1				; j loop
	LDRB R8, [R7, R6]	; player->pieces_hit[i][j]
	CMP R8, R9			; compare
	BNE NOINCRN
	ADD R4, R4, #1		; n++
NOINCRN
	ADD R6, R6, #1		; j++
	RSB R8, R5, #4		; 4-i
	CMP R6, R8			; j<4-i
	BLT J_LOOP_1			
	; END J LOOP
	ADD R5, R5, #1		; i++
	CMP R5, #4			; i<4
	BLT I_LOOP_1			
	; END I LOOP
	
	B RETURN_1			; unconditional branching to RETURN
		
RETURN_1
	MOV	R0, R4		
	POP {R4-R9}	;
	BX	LR				; return 0
  ENDFUNC
  
  EXPORT player_done_s

player_done_s FUNCTION	; char player_done_s(player_t * player);
	PUSH {R4-R9}
	; r0 *player
	ADD R0, R0, #6		; player->pieces_hit
	MOV R4, #1			; won = 1
	MOV R5, #0			; i =0
I_LOOP	 				; i loop
	ADD R9, R5, #1		; R9 <- i + 1
	LSL R7, R5, #2		; 
	ADD R7, R0, R7		; r7 <- pieces_hit[i]
	MOV R6, #0			; j =0
J_LOOP					; j loop
	LDRB R8, [R7, R6]	; player->pieces_hit[i][j]
	CMP R8, R9			; compare
	BLT NOTWON
	ADD R6, R6, #1		; j++
	RSB R8, R5, #4		; 4-i
	CMP R6, R8			; j<4-i
	BLT J_LOOP			
	; END J LOOP
	ADD R5, R5, #1		; i++
	CMP R5, #4			; i<4
	BLT I_LOOP			
	; END I LOOP
	
	B RETURN			; unconditional branching to RETURN
	
NOTWON			
	MOV R4, #0
	
RETURN
	MOV	R0, R4
	POP {R4-R9}		
	BX	LR				; return 0
  ENDFUNC
  
  END