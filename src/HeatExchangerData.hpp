#ifndef HEATEXCHANGERDATA_HPP_
#define HEATEXCHANGERDATA_HPP_

#include "JSONData.hpp"

class HeatExchangerData: public JSONData {
public:
	void insertLastIntoDB() override;
	void initDBTable() override;
};

#endif /* HEATEXCHANGERDATA_HPP_ */
