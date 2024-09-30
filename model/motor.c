#include "config.h"
#include "bsp.h"
#include "motor.h"

/* 变量 --------------------------------------------------------------------*/
static struct mt_tx_t   mt_tx;

/* 硬件接口 --------------------------------------------------------------------*/
static void mt_hardware_init( void )
{
    MT_GPIO_TX_MODE;
    MT_GPIO_RX_MODE;
    
    MT_GPIO_TX_RESET;
}

/* 数据传输函数 --------------------------------------------------------------------*/
void mt_init( void )
{
    mt_hardware_init();

    memset( &mt_tx, NULL, sizeof(struct mt_tx_t) );
}

int mt_tx_data_single( uint8_t stbit, uint8_t order, uint8_t cdata )
{
    if( mt_tx.active != MT_TX_ACTIVE_EMPTY ) return -1;

    mt_tx.cnt = 0;
    mt_tx.index = 13;

    /* 发送参数 */
    mt_tx.tdata.buff = 0;
    mt_tx.tdata.fmt.stbit = stbit;
    mt_tx.tdata.fmt.order = order;
    mt_tx.tdata.fmt.cdata = cdata;

    /* 接收参数 */
    mt_tx.rdata.buff = 0;

    /* 启动发送 */
    mt_tx.active = MT_TX_ACTIVE_START;

    // printf( "mt-tx: %04X, order: %bX, cdata: %02bX\r\n", mt_tx.tdata.buff, order, cdata );
    
    return 0;
}

void mt_process( void )
{
    if( mt_tx.active == MT_TX_ACTIVE_DONE )
    {
        /* 处理数据 */
        mt_rx_data_process( mt_tx.rdata );
        
        /* 清空标志位 */
        mt_tx.active = MT_TX_ACTIVE_EMPTY;

        // printf( "MT send done, %04X\r\n", mt_tx.rdata );
    }
}

void mt_tx_bit_toggle( uint8_t status )
{
    mt_tx.active = MT_TX_ACTIVE_FREEZE;
    
    if (status) MT_GPIO_TX_SET;
        else MT_GPIO_TX_RESET;

    // printf("mt tx toggle\r\n");
}

void mt_tx_bit_toggle_free( void )
{
    if (mt_tx.active == MT_TX_ACTIVE_FREEZE)
        mt_tx.active = MT_TX_ACTIVE_EMPTY;
    
    // printf("mt tx toggle free\r\n");
}

// 0.5ms 定时服务处理函数
void mt_timer_isr( void )
{
    if( mt_tx.active == MT_TX_ACTIVE_START )
    {
        /* 第一位分3个脉宽 */
        switch( mt_tx.cnt )
        {
            case 0: 
                {
                    mt_tx.rdata.buff = (mt_tx.rdata.buff << 1) | MT_GPIO_RX_READ;

                    if( --mt_tx.index < 0 ) mt_tx.active = MT_TX_ACTIVE_DONE;

                    MT_GPIO_TX_RESET;
                } break;
            case 1: 
                {
                    if( (mt_tx.tdata.buff >> mt_tx.index) & 1 ) MT_GPIO_TX_RESET;
                        else MT_GPIO_TX_SET;
                } break;
            case 2: 
                {
                    MT_GPIO_TX_SET;
                } break;
            default: break;
        }

        mt_tx.cnt = ++mt_tx.cnt % 3;
    }
}
