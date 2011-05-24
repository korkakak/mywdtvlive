
int wpi_encrypt(unsigned char * pofbiv_in,
				unsigned char * pbw_in,
				unsigned int plbw_in,
				unsigned char * pkey,
				unsigned char * pcw_out);

int wpi_decrypt(unsigned char * pofbiv_in,
				unsigned char * pcw_in,
				unsigned int plcw_in,
				unsigned char * prkey_in,
				unsigned char * pbw_out);

int wpi_pmac(unsigned char * pmaciv_in,
				unsigned char * pmac_in,
				unsigned int pmacpc_in,
				unsigned char * pkey,
				unsigned char * pmac_out);

int wpi_cbc_mac_engine(
		unsigned char * maciv_in,
		unsigned char * in_data1,
		unsigned int 	in_data1_len,
		unsigned char * in_data2,
		unsigned int 	in_data2_len,
		unsigned char * pkey,
		unsigned char * mac_out);

int wpi_sms4_ofb_engine(
	unsigned char * pofbiv_in,
	unsigned char * pbw_in,
	unsigned int 	plbw_in,
	unsigned char * pkey,
	unsigned char * pcw_out);


