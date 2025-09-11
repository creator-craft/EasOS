#ifndef ATA_H
#define ATA_H

#include "types.h"

struct ATA_DEVICE_INFORMATION {
  u16 general_configuration_bit;
  u16 obs1;
  u16 specific_configurarion;
  u16 obs2, ret1[2], obs3, res1[2], ret2;
  u8 serial_number[20];
  u16 ret3[2], obs4;
  u8 firmware_revision[8], model_number[40];
  u16 max_sectors_transfert; // 0x8000 + (u8)sector_count
  u16 res2;
  u16 capabilities1, capabilities2;
  u16 obs5[2];
  u16 fields_validity;
  u16 obs6[5];
  u16 multiple_sector_setting;
  u32 total_addressable_sectors;
  u16 obs7;
  u16 multiword_dma_mode;
  u16 pio_modes_supported;
  u16 min_multiword_dma_transfert_cycle_time_per_word;
  u16 manifacturer_recomended_multiword_dma_transfert_cycle_time;
  u16 min_pio_transfert_cycle_time_without_control;
  u16 min_pio_transfert_cycle_time_with_iordy_flow_control;
  u16 res3[2], res4[4];
  u16 queue_depth;
  u16 res5[4];
  u16 major_version_number, minor_version_number;
  u16 supported_commands1, supported_commands2, supported_commands_ext;
  u16 enabled_commands1, enabled_commands2, command_default;
  u16 ultra_dma_mode;
  u16 erase_required_time, enhanced_erase_required_time;
  u16 current_advanced_power_management_value;
  u16 master_password_revision_code;
  u16 hardware_reset_result;
  u16 accoustic_management;
  u16 res6[5];
  u64 maximum_user_lba_for_48bit_address_feature_set;
  u16 res7[23];
  u16 removable_media_status;
  u16 security_status;
  u16 vendor_specific[31];
  u16 cfa_power_mode;
  u16 res8[15];
  u16 current_media_serial_number[30];
  u16 res9[49];
  u8 signature;
  u8 checksum;

} __attribute__((packed, aligned(2)));

enum devices {
  NO_DEVICE, UNKNOW_DEVICE, PATAPI_DEVICE, SATAPI_DEVICE, PATA_DEVICE, SATA_DEVICE
};

u8 identify(u8 device, struct ATA_DEVICE_INFORMATION *info_ptr);

#endif // ATA_H