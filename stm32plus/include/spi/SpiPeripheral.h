/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * SPI peripheral class
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  class SpiPeripheral : public Spi {

    public:

      struct Parameters {
        uint16_t spi_direction;
        uint16_t spi_mode;
        uint16_t spi_baudRatePrescaler;
        uint16_t spi_firstBit;
        uint16_t spi_cpol;
        uint16_t spi_cpha;
        uint16_t spi_polynomial;

        Parameters() {
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

      SpiPeripheral(const Parameters& params)
        : Spi((SPI_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE,
              (GPIO_TypeDef *)TPinPackage::Port_NSS,
              TPinPackage::Pin_NSS,
              params.spi_direction) {

        SPI_InitTypeDef init;

        // clocks on first before any features get initialised

        ClockControl<TPeripheralName>::On();

        // and next the GPIO pins, also before any features get initialised

        SpiPinInitialiser<TPinPackage,PeripheralTraits<TPeripheralName> >::initialise(params.spi_mode);

        // initialise the peripheral

        SPI_StructInit(&init);

        init.SPI_Direction=params.spi_direction;
        init.SPI_Mode=params.spi_mode;
        init.SPI_DataSize=SPI_DataSize_8b;
        init.SPI_CPOL=params.spi_cpol;
        init.SPI_CPHA=params.spi_cpha;
        init.SPI_NSS=SPI_NSS_Soft;
        init.SPI_BaudRatePrescaler=params.spi_baudRatePrescaler;
        init.SPI_FirstBit=params.spi_firstBit;
        init.SPI_CRCPolynomial=params.spi_polynomial;

        SPI_Init(_peripheralAddress,&init);

        // NSS is soft, ensure it's high to begin with

        setNss(true);

        // enable SPI

        enablePeripheral();
      }
  };
}
