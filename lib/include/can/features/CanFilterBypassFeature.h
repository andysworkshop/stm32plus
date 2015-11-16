#pragma once


namespace stm32plus {

  /**
   * Feature class for bypass CAN filter
   */

  struct CanFilterBypassFeature : public CanFeatureBase {


	  /**
	   * Constructor
	   * @param can
	   */

	  CanFilterBypassFeature(Can& can)
	  : CanFeatureBase(can) {

	  }

	  /**
	   * Post CAN_Init() initialisation
	   */

	  void initialise() {
		CAN_FilterInitTypeDef CANFilterInitStruct;
		CANFilterInitStruct.CAN_FilterNumber = 0;
		CANFilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
		CANFilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
		CANFilterInitStruct.CAN_FilterIdHigh = 0x0000;
		CANFilterInitStruct.CAN_FilterIdLow = 0x0000;
		CANFilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
		CANFilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
		CANFilterInitStruct.CAN_FilterFIFOAssignment = 0;
		CANFilterInitStruct.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&CANFilterInitStruct);
	  }

  };
}
