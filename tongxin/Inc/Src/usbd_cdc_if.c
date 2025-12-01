/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v1.0_Cube
  * @brief          : USB设备虚拟串口实现
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
static USBCallback tx_cbk = NULL;  // 发送完成回调函数
static USBCallback rx_cbk = NULL;  // 接收完成回调函数
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief USB设备库
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief 私有类型定义
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief 私有定义
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief 私有宏定义
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief 私有变量
  * @{
  */
/* 创建接收和发送缓冲区 */
/* 用户可以重定义和/或移除这些定义 */
/** 通过USB接收的数据存储在这个缓冲区中 */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** 通过USB CDC发送的数据存储在这个缓冲区中 */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
uint8_t  myUsbRxData[64] = { 0 };   // 接收到的数据
uint16_t myUsbRxNum = 0;            // 接收到的字节数
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief 公共变量
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief 私有函数声明
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);
static int8_t CDC_TransmitCplt_FS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS,
  CDC_TransmitCplt_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  初始化CDC介质底层 over FS USB IP
  * @retval USBD_OK 如果所有操作都成功，否则返回 USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* 设置应用缓冲区 */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  反初始化CDC介质底层
  * @retval USBD_OK 如果所有操作都成功，否则返回 USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  管理CDC类请求
  * @param  cmd: 命令代码
  * @param  pbuf: 包含命令数据的缓冲区（请求参数）
  * @param  length: 要发送的数据长度（字节）
  * @retval 操作结果: USBD_OK 如果所有操作都成功，否则返回 USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:  // 发送封装命令

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:  // 获取封装响应

    break;

    case CDC_SET_COMM_FEATURE:           // 设置通信特性

    break;

    case CDC_GET_COMM_FEATURE:           // 获取通信特性

    break;

    case CDC_CLEAR_COMM_FEATURE:         // 清除通信特性

    break;

  /*******************************************************************************/
  /* 线路编码结构                                                                */
  /*-----------------------------------------------------------------------------*/
  /* 偏移量 | 字段名      | 大小 | 值    | 描述                                  */
  /* 0      | dwDTERate   |   4  | 数字  | 数据终端速率，单位比特每秒            */
  /* 4      | bCharFormat |   1  | 数字  | 停止位                               */
  /*                                        0 - 1 停止位                        */
  /*                                        1 - 1.5 停止位                      */
  /*                                        2 - 2 停止位                        */
  /* 5      | bParityType |  1   | 数字  | 校验位                               */
  /*                                        0 - 无校验                          */
  /*                                        1 - 奇校验                          */
  /*                                        2 - 偶校验                          */
  /*                                        3 - 标记校验                        */
  /*                                        4 - 空格校验                        */
  /* 6      | bDataBits  |   1   | 数字  | 数据位 (5, 6, 7, 8 或 16)            */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:            // 设置线路编码

    break;

    case CDC_GET_LINE_CODING:            // 获取线路编码

    break;

    case CDC_SET_CONTROL_LINE_STATE:     // 设置控制线状态

    break;

    case CDC_SEND_BREAK:                 // 发送中断

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  通过USB OUT端点接收的数据通过此函数发送到CDC接口
  *
  *         @note
  *         此函数将在任何OUT数据包上发出NAK包，直到退出此函数。
  *         如果在CDC接口上的传输未完成（例如使用DMA控制器）时退出此函数，
  *         将导致在先前数据尚未发送时接收更多数据。
  *
  * @param  Buf: 要接收的数据缓冲区
  * @param  Len: 接收的数据长度（字节）
  * @retval 操作结果: USBD_OK 如果所有操作都成功，否则返回 USBD_FAIL
  */
// static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
// {
//   /* USER CODE BEGIN 6 */
//   char myStr[64] = {0};                                                     // 定义一个数组，用于存放要输出的字符串
//   sprintf(myStr, "\r\r收到 %d 个字节；\r内容是：%s\r\r", *Len, (char *)Buf); // 格式化字符串
//   CDC_Transmit_FS((uint8_t *)myStr, strlen(myStr));                         // 发送
//   memset(Buf, 0, 64);                                                       // 处理完数据，清0接收缓存;
//
//   USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
//   USBD_CDC_ReceivePacket(&hUsbDeviceFS);
//
//   if(rx_cbk)
//     rx_cbk(*Len);  // 调用接收完成回调函数
//
//   return (USBD_OK);
//   /* USER CODE END 6 */
// }

// static int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len)
// {
//   /* USER CODE BEGIN 6 */
//   // 把Buf里面的数据，复制到外部缓存
//   memset(myUsbRxData, 0, 64);                     // 清0缓存区
//   memcpy(myUsbRxData, Buf, *Len);                 // 把接收到的数据，复制到自己的缓存区中
//   myUsbRxNum = *Len;                              // 复制字节数
//   memset(Buf, 0, 64);                             // 处理完数据，清0接收缓存;
//
//   // CubeMX生成的代码，保留
//   USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);   // 设置下-个接收缓冲区
//   USBD_CDC_ReceivePacket(&hUsbDeviceFS);          // 启动下一个数据包的接收
//   return (USBD_OK);
//   /* USER CODE END 6 */
// }

static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  command_t cmd_tmp;//定义临时结构体，校验后再传给cmd
  if(*Len == sizeof(command_t))//长度校验
    memcpy(&cmd_tmp,Buf,sizeof(command_t));

  uint16_t crc_code = 0;
  crc_code = MBCRC16((uint8_t*)&cmd_tmp,sizeof(command_t)-2);//-2 去掉最后两个CRC字节
  if(cmd_tmp.crc == crc_code){
    memcpy(&cmd,Buf,sizeof(command_t));
  }
  /* USER CODE BEGIN 6 */
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         通过USB IN端点发送的数据通过此函数发送到CDC接口
  *         @note
  *
  *
  * @param  Buf: 要发送的数据缓冲区
  * @param  Len: 要发送的数据长度（字节）
  * @retval USBD_OK 如果所有操作都成功，否则返回 USBD_FAIL 或 USBD_BUSY
  */
// uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
// {
//   uint8_t result = USBD_OK;
//   /* USER CODE BEGIN 7 */
//   USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
//   if (hcdc->TxState != 0){
//     return USBD_BUSY;  // 如果正在发送，返回忙状态
//   }
//   USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
//   result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
//   /* USER CODE END 7 */
//   return result;
// }

uint8_t CDC_Transmit_FS(uint8_t *Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */

  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)hUsbDeviceFS.pClassData; // 获得设备的状态信息结构体
  // if (hcdc->TxState != 0){
  // return USBD_BUSY;
  // }
  uint32_t timeStart = HAL_GetTick();
  while (hcdc->TxState)
  {
    if (HAL_GetTick() - timeStart > 20)
      return USBD_BUSY;
  }

  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);

  /* USER CODE END 7 */
  return result;
}

// uint8_t CDC_Transmit_FS(uint8_t *Buf, uint16_t Len)
// {
//   USBD_CDC_HandleTypeDef *hcdc =
//       (USBD_CDC_HandleTypeDef *)hUsbDeviceFS.pClassData;
//
//   uint32_t timeStart = HAL_GetTick();
//
//   // 等待 USB 空闲
//   while (hcdc->TxState != 0)
//   {
//     if (HAL_GetTick() - timeStart > 100)   // 延长到 100ms
//       return USBD_FAIL;                  // 不返回 BUSY !!
//     HAL_Delay(1);
//   }
//
//   USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
//
//   uint8_t result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
//
//   // 等待发送包被 USB 层接收
//   timeStart = HAL_GetTick();
//   while (hcdc->TxState != 0)
//   {
//     if (HAL_GetTick() - timeStart > 100)
//       return USBD_FAIL;
//     HAL_Delay(1);
//   }
//
//   return result;
// }

/**
  * @brief  CDC_TransmitCplt_FS
  *         数据传输完成回调
  *
  *         @note
  *         此函数是IN传输完成回调，用于通知用户提交的数据已成功通过USB发送。
  *
  * @param  Buf: 要接收的数据缓冲区
  * @param  Len: 接收的数据长度（字节）
  * @retval 操作结果: USBD_OK 如果所有操作都成功，否则返回 USBD_FAIL
  */
static int8_t CDC_TransmitCplt_FS(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 13 */
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);
  if(tx_cbk)
    tx_cbk(*Len);  // 调用发送完成回调函数
  /* USER CODE END 13 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @brief  初始化CDC接收缓冲区和回调函数
  * @param  transmit_cbk: 发送完成回调函数
  * @param  recv_cbk: 接收完成回调函数
  * @retval 接收缓冲区指针
  */
uint8_t* CDCInitRxbufferNcallback(USBCallback transmit_cbk,USBCallback recv_cbk)
{
  tx_cbk = transmit_cbk;
  rx_cbk = recv_cbk;
  return UserRxBufferFS;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */