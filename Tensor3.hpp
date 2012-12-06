#ifndef __TENSOR3_H__
#define __TENSOR3_H__

#include "MathIO.hpp"
#include "Eigen/Dense" 


class Tensor3 {
	
	public:
		Tensor3();
		
	protected:
	private:
		MatrixXd L(3,3);
		Eigen::MatrixXd M(3,3);
		Eigen::MatrixXd N(3,3);
};

/* Access at element of Tensor, T(i,j,k)
 *
 */
int Tensor3::operator()(unsigned int i, unsigned int j, unsigned int k) 
{
	int res = 123456789;
	switch(k) {
		case 0:
			res = L.(i,j);
		break;
		case 1:
			res = L.(i,j);
		break;
		case 2:
			res = L.(i,j);
		break;
		default: // Only 3 x 3 x 3 tensor, but user will never use this directly.
			res = -123456789;
		break	;	
	}
	
	return res;
}


#endif //__TENSOR_H__