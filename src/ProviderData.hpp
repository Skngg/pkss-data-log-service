#ifndef SRC_PROVIDERDATA_HPP_
#define SRC_PROVIDERDATA_HPP_

#include "JSONData.hpp"

class ProviderData: public JSONData {
	void insertLastIntoDB() override;
	void initDBTable() override;
};

#endif /* SRC_PROVIDERDATA_HPP_ */
