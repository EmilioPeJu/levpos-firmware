#include <stdint.h>

#include "peripherals.h"
#include "serial.h"

struct ring_buffer host_serial_buffer;

bool serial_init()
{
    HOST_HUART.Instance->CR1 |= USART_CR1_RXNEIE;
    return true;
}

void serial_handle_rx()
{
    uint32_t isrflags = READ_REG(HOST_HUART.Instance->SR);
    if (isrflags & USART_SR_RXNE) {
        rb_put_isr(&host_serial_buffer, READ_REG(HOST_HUART.Instance->DR));
        if (rb_is_empty(&host_serial_buffer)) {
            host_serial_buffer.buffer_overruns++;
        }
    }
    if (isrflags & USART_SR_ORE) {
        host_serial_buffer.uart_overruns++;
    }
}

int _write(int file, char *ptr, int len)
{
    if (len < 0 || len >= 65536)
        return -1;

    return HAL_UART_Transmit(
        &HOST_HUART, (uint8_t *) ptr, (uint16_t) len,
        TX_SERIAL_TIMEOUT) == HAL_OK ? len : -1;
}
