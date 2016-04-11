#include "app_common.h"
#include "app_ipi_common.h"

BU32 APP_CA_API_GetCardIn(void)
{
	return FYF_API_ca_get_value(FYF_CA_SV_CARD_IN,0,0);
}
BU32 APP_CA_API_GetValue(FYF_CA_SV_e type,BU32 *para1,BU32 *para2)
{
	return FYF_API_ca_get_value(type,para1,para2);
}
