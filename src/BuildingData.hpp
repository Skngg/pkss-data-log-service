#ifndef SRC_BUILDINGDATA_HPP_
#define SRC_BUILDINGDATA_HPP_

#include "JSONData.hpp"

class BuildingData: public JSONData {
public:
	void insertLastIntoDB() override;
	void initDBTable() override;
};

#endif /* SRC_BUILDINGDATA_HPP_ */
