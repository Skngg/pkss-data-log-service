#ifndef SRC_CONTROLLERDATA_HPP_
#define SRC_CONTROLLERDATA_HPP_

#include "JSONData.hpp"

class ControllerData: public JSONData {
public:
	void insertLastIntoDB() override;
	void initDBTable() override;
};

#endif /* SRC_CONTROLLERDATA_HPP_ */
