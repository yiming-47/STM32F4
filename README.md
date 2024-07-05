只是個簡單demo，DMA傳輸部分尚未完成、畫圖(LCD)尚未完成  
環境設置:  
  IDE: TrueStudo 7.3.0 (用git後排版變很奇怪 請見諒)  
  板子型號: STM32F429DISC1 
硬體型號:  
  觸控功能:STMPE811 4線電阻式觸控  
  LCD: ILI9341 
腳位設置:
  TouchPanel: PA7 (RESET), PA15(TOUCHPANEL_INTERRUPT)    
  LCD: PD13 (DATA_COMMAND_SELECT)
  USART: PA9, PA10 (只是用來debug與呈現觸控位置)  
  SPI: PC2(SS), PF7, PF8, PF9  
  I2C: PA8, PC9  
