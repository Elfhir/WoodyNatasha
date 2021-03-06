/*
 *
 *
 *
 */
 
#include "Tensor3d.hpp"
#include "MathIO.hpp"

Tensor3d::Tensor3d(MatrixXd L1, MatrixXd L2, MatrixXd L3)
{
	// Tensor3d : 3 x 3 x 3   i.e 3 matrix 3x3, initialized with 0
	this->L = MatrixXd::Zero(3,3);
	this->M = MatrixXd::Zero(3,3);
	this->N = MatrixXd::Zero(3,3);

	// Just initialization, not useful.
	this->Adouble = buildMatrixA(L1, L2, L3);

}

// Setters and Getters.
MatrixXd Tensor3d::getL() {return L;}
MatrixXd Tensor3d::getM() {return M;}
MatrixXd Tensor3d::getN() {return N;}

void Tensor3d::setL(MatrixXd L1) { L = L1;}
void Tensor3d::setM(MatrixXd L2) { M = L2;}
void Tensor3d::setN(MatrixXd L3) { N = L3;}

void Tensor3d::setLij(unsigned int i, unsigned int j, double tvalue) {L(i,j) = tvalue;}
void Tensor3d::setMij(unsigned int i, unsigned int j, double tvalue) {M(i,j) = tvalue;}
void Tensor3d::setNij(unsigned int i, unsigned int j, double tvalue) {N(i,j) = tvalue;}

// L, M and N have the same size.
int Tensor3d::getRows() {
	return L.rows();
}


// getVal and operator(,,) will never be used directly by user, so a case
// witk k > 2 will never happen. These functions are called in buildMatrixA
// and transferToX

/* Access at element of Tensor, getVal(i,j,k)
 * return double
 */
double Tensor3d::getVal(unsigned int i, unsigned int j, unsigned int k) {
	double res = 123456789;
	MatrixXd tmp;
	switch(k) {
		case 0:
			tmp = getL();
			res = tmp(i,j);
		break;
		case 1:
			tmp = getM();
			res = tmp(i,j);
		break;
		case 2:
			tmp = getN();
			res = tmp(i,j);
		break;
		default:
			res = -123456789;
		break	;	
	}	
			
	return res;
}

/* Access at element of Tensor, T(i,j,k)
 * return double
 */
double Tensor3d::operator()(unsigned int i, unsigned int j, unsigned int k) {
	int res = 123456789;
	switch(k) {
		case 0:
			res = L(i,j);
		break;
		case 1:
			res = M(i,j);
		break;
		case 2:
			res = N(i,j);
		break;
		default:
			res = -123456789;
		break;	
	}	
	
	return res;
}


// Fill Each values of matrix L, M and N of Tensor3d.
void Tensor3d::setVal(VectorXd t) {
	for(int i = 0; i<27 ; ++i) {
		if(i<9)
			if(i<3)
				setLij(0,i,t(i));
			else if(i >= 3 && i < 6)
				setLij(1,i-3,t(i));
			else if(i >= 6 && i < 9)
				setLij(2,i-6,t(i));
			else{}

		else if(i >= 9 && i<18)
			if(i >= 9 && i<12)
				setMij(0,i-9,t(i));
			else if(i >= 12 && i < 15 )
				setMij(1,i-12,t(i));
			else if(i >= 15 && i < 15)
				setMij(2,i-15,t(i));
			else{}
			
		else if(i >= 18 && i<27) 
			if(i >= 18 && i<21)
				setNij(0,i-18,t(i));
			else if(i >= 21 && i < 24)
				setNij(1,i-21,t(i));
			else if(i >= 24 && i < 27)
				setNij(2,i-24,t(i));
			else{}
		else {}
	}
	
	return;
}


// Just show the tensor value.
void Tensor3d::printTensor3d() {
	std::cout << "   Tensor3d :" << std::endl;
	for(int k = 0; k<3 ; ++k) {
		for(int i = 0; i<getRows() ; ++i) {		
				std::cout << " " << getVal(i,0,k) << " " << getVal(i,1,k) << " " << getVal(i,2,k) << std::endl;
		}
		std::cout << std::endl;
	}

}

// Function buildMatrixA, for solvind a system using SVD
Eigen::MatrixXd Tensor3d::buildMatrixA(MatrixXd L1, MatrixXd L2, MatrixXd L3) {
	int points = L1.rows();
	int limit = 4*points;
	Eigen::MatrixXd A = MatrixXd::Zero(limit, 27);
	
	// For each points
	for(int p = 0; p < points; ++p) {

		// For each coords of a points - seems like playing with x,y,z
		for(int k = 0; k<3 ; ++k) {

			// 4 equations - seems like playing with x and y values combination - xx, xy, yx, yy -
			for(int i = 0; i<2; ++i) {
				for(int l = 0; l<2; ++l) {

					if(i==0 && l==0) {
						A(4*p + 2*i + l, 6+k*9) = L1(p,k) * (  L2(p,i)*L3(p,2) );
						A(4*p + 2*i + l, 0+k*9) = L1(p,k) * ( -L2(p,2)*L3(p,2) );
						A(4*p + 2*i + l, 8+k*9) = L1(p,k) * ( -L2(p,i)*L3(p,l) );
						A(4*p + 2*i + l, 2+k*9) = L1(p,k) * (  L2(p,2)*L3(p,l) );
					}
					else if(i==0 && l==1) {
						A(4*p + 2*i + l, 7+k*9) = L1(p,k) * (  L2(p,i)*L3(p,2) );
						A(4*p + 2*i + l, 1+k*9) = L1(p,k) * ( -L2(p,2)*L3(p,2) );
						A(4*p + 2*i + l, 8+k*9) = L1(p,k) * ( -L2(p,i)*L3(p,l) );
						A(4*p + 2*i + l, 2+k*9) = L1(p,k) * (  L2(p,2)*L3(p,l) );
					}
					else if(i==1 && l==0) {
						A(4*p + 2*i + l, 6+k*9) = L1(p,k) * (  L2(p,i)*L3(p,2) );
						A(4*p + 2*i + l, 3+k*9) = L1(p,k) * ( -L2(p,2)*L3(p,2) );
						A(4*p + 2*i + l, 8+k*9) = L1(p,k) * ( -L2(p,i)*L3(p,l) );
						A(4*p + 2*i + l, 5+k*9) = L1(p,k) * (  L2(p,2)*L3(p,l) );	
					}
					else if(i==1 && l==1) {
						A(4*p + 2*i + l, 7+k*9) = L1(p,k) * (  L2(p,i)*L3(p,2) );
						A(4*p + 2*i + l, 4+k*9) = L1(p,k) * ( -L2(p,2)*L3(p,2) );
						A(4*p + 2*i + l, 8+k*9) = L1(p,k) * ( -L2(p,i)*L3(p,l) );
						A(4*p + 2*i + l, 5+k*9) = L1(p,k) * (  L2(p,2)*L3(p,l) );	
					}
					else {}
	
				} // end for l
			} // end for i
 
		} // end for k

	} // end for p

	return A;
}


/* Each following transfer are called at the good place in the main.cpp
 * They solved a system using SVD.
 *
 */

Eigen::MatrixXd Tensor3d::transfertTo1(Eigen::MatrixXd LI, Eigen::MatrixXd LJ) {
	Eigen::MatrixXd A2 = MatrixXd::Zero(4,2);
	Eigen::VectorXd X2 = MatrixXd::Zero(2,1);
	Eigen::VectorXd b = MatrixXd::Zero(4,1);

	int end = LI.rows() - 1 ;
	
	for(int i = 0; i<2; ++i) {
		for(int l = 0; l<2; ++l) {
				
			if(i==0 && l==0) {
				A2(2*i + l, 0) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,0) - LI(end,2)*getVal(i,l,0 ) ) );
				A2(2*i + l, 1) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,1) - LI(end,2)*getVal(i,l,1 ) ) );
			}
			else if(i==0 && l==1) {
				A2(2*i + l, 0) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,0) - LI(end,2)*getVal(i,l,0 ) ) );
				A2(2*i + l, 1) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,1) - LI(end,2)*getVal(i,l,1 ) ) );
			}
			else if(i==1 && l==0) {
				A2(2*i + l, 0) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,0) - LI(end,2)*getVal(i,l,0 ) ) );
				A2(2*i + l, 1) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,1) - LI(end,2)*getVal(i,l,1 ) ) );
			}
			else if(i==1 && l==1) {
				A2(2*i + l, 0) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,0) - LI(end,2)*getVal(i,l,0 ) ) );
				A2(2*i + l, 1) = ( LJ(end,2) * ( LI(end,i)*getVal(2,l,1) - LI(end,2)*getVal(i,l,1 ) ) );
			}
			else {}

			b(2*i + l) =  -( LI(end,i)*LJ(end,2)*getVal(2,l,2) - LI(end,2)*LJ(end,2)*getVal(i,l,2) - LI(end,i)*LJ(end,l)*getVal(2,2,2) + LI(end,2)*LJ(end,l)*getVal(i,2,2) );
		}
			
	}

	// SVD decomposition of A2
	JacobiSVD<MatrixXd> svd(A2, ComputeThinU | ComputeThinV);

	X2 = svd.solve(b);
	
	return X2;
}

 Eigen::MatrixXd Tensor3d::transfertTo3(Eigen::MatrixXd LI, Eigen::MatrixXd LJ) {
	Eigen::MatrixXd A2 =  MatrixXd::Zero(4,2);
	Eigen::VectorXd X2 = MatrixXd::Zero(2,1);
	Eigen::VectorXd b = MatrixXd::Zero(4,1);

	int end = LI.rows() - 1 ;

	for(int k = 0; k<3; ++k) {

		for(int i = 0; i<2; ++i) {
			for(int l = 0; l<2; ++l) {
				
				A2(2*i + l, l) += ( LI(end,k) * ( LJ(end,2)*getVal(i,2,k) - LJ(end,i)*getVal(2,2,k) ) );
				b(2*i + l) -=  LI(end,k) * ( LJ(end,i) * getVal(2,l,k) - LJ(end,2) * getVal(i,l,k) );

			}
				
		}

	}

	// SVD decomposition of A2
	JacobiSVD<MatrixXd> svd(A2, ComputeThinU | ComputeThinV);

	X2 = svd.solve(b);

	return X2;
}


 Eigen::MatrixXd Tensor3d::transfertTo2(Eigen::MatrixXd LI, Eigen::MatrixXd LJ) {
	Eigen::MatrixXd A2 =  MatrixXd::Zero(4,2);
	Eigen::VectorXd X2 = MatrixXd::Zero(2,1);
	Eigen::VectorXd b = MatrixXd::Zero(4,1);

	int end = LI.rows() - 1 ;

	for(int k = 0; k<3; ++k) {

		for(int i = 0; i<2; ++i) {
			for(int l = 0; l<2; ++l) {
				
				A2(2*i + l, i) += ( LI(end,k) * ( LJ(end,2)*getVal(2,l,k) - LJ(end,i)*getVal(2,2,k) ) );
				b(2*i + l) -=  LI(end,k) * ( LJ(end,l)*getVal(i,2,k) - LJ(end,2) * getVal(i,l,k) );
				
			}
				
		}

	}

	// SVD decomposition of A2
	JacobiSVD<MatrixXd> svd(A2, ComputeThinU | ComputeThinV);

	X2 = svd.solve(b);
	
	return X2;
 }
 
 