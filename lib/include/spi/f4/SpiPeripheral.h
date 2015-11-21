/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * SPI peripheral base class
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  class SpiPeripheral : public Spi {

    public:

      /**
       * Parameters class
       */

      struct Parameters {
        uint16_t spi_direction;             ///< (SPI_Direction_2Lines_FullDuplex) / SPI_Direction_2Lines_RxOnly / SPI_Direction_1Line_Rx / SPI_Direction_1Line_Tx
        uint16_t spi_mode;                  ///< (SPI_Mode_Master) / SPI_Mode_Slave
        uint16_t spi_baudRatePrescaler;     ///< SPI_BaudRatePrescaler_2 / 4 / 8 / (16) / 32 / 64 / 128 / 256
        uint16_t spi_firstBit;              ///< (SPI_FirstBit_MSB) / SPI_FirstBit_LSB
        uint16_t spi_cpol;                  ///< (SPI_CPOL_High) SPI_CPOL_Low
        uint16_t spi_cpha;                  ///< (SPI_CPHA_2Edge) / SPI_CPHA_1Edge
        uint16_t spi_polynomial;            ///< default is 7
        uint16_t spi_dataSize;              ///< (SPI_DataSize_8b) / SPI_DataSize_16b (more available on the F0)

        /**
         * Constructor, set suitable defaults for master mode. Generally you'll want to set at
         * least spi_baudRatePrescaler to a suitable speed and check that the other defaults are OK.
         */

        Parameters() {
          spi_dataSize=SPI_DataSize_8b;
          spi_direction=SPI_Direction_2Lines_FullDuplex;
          spi_mode=SPI_Mode_Master;
          spi_baudRatePrescaler=SPI_BaudRatePrescaler_16;
          spi_firstBit=SPI_FirstBit_MSB;
          spi_cpol=SPI_CPOL_High;
          spi_cpha=SPI_CPHA_2Edge;
          spi_polynomial=7;
        }
      };

    protected:
      SpiPeripheral(const Parameters& params);
      ~SpiPeripheral();
  };


  /**
   * Constructor
   * @param params The initialisation parameters
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline SpiPeripheral<TPinPackage,TPeripheralName>::SpiPeripheral(const Parameters& params)
    : Spi((SPI_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE,
          (GPIO_TypeDef *)TPinPackage::Port_NSS,
          TPinPackage::Pin_NSS,
          params.spi_direction) {

    SPI_InitTypeDef init;

    // clocks on first before any features get initialised

    ClockControl<TPeripheralName>::On();

    // and next the GPIO pins, also before any features get initialised

    SpiPinInitialiser<TPinPackage,TPeripheralName>::initialise(params.spi_mode,params.spi_direction);

    // initialise the peripheral

    SPI_StructInit(&init);

    init.SPI_Direction=params.spi_direction;
    init.SPI_Mode=params.spi_mode;
    init.SPI_DataSize=params.spi_dataSize;
    init.SPI_CPOL=params.spi_cpol;
    init.SPI_CPHA=params.spi_cpha;
    init.SPI_NSS=SPI_NSS_Soft;
    init.SPI_BaudRatePrescaler=params.spi_baudRatePrescaler;
    init.SPI_FirstBit=params.spi_firstBit;
    init.SPI_CRCPolynomial=params.spi_polynomial;

    SPI_Init(_peripheralAddress,&init);

    // NSS is soft, ensure it's high to begin with

    setNss(true);

    // enable SPI if we're doing 4-wire full duplex in 3-wire mode enable/disable is manual

    if(params.spi_direction==SPI_Direction_2Lines_FullDuplex)
      enablePeripheral();
  }


  /**
   * Destructor
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline SpiPeripheral<TPinPackage,TPeripheralName>::~SpiPeripheral() {

    // disable peripheral

    disablePeripheral();

    // clocks off

    ClockControl<TPeripheralName>::Off();
  }
}
