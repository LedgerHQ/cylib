##*************************************************************************************/
##/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project	 */
##/* License: This software is licensed under a dual BSD and GPL v2 license. 	 */
##/* See LICENSE file at the root folder of the project.				 */
##/* FILE: musig2.sage							         */
##/* 											 */
##/* 											 */
##/* DESCRIPTION: 2 round_multisignature Musig2 signatures verification smart contract*/
##/source code 		 */
##/* https:##eprint.iacr.org/2020/1261.pdf             				 */
##/* note that some constant aggregating values could be precomputed			 */
##**************************************************************************************/
##** sagemath is a usefull algebraic toolbox, you may install it or run directly the following lines without the "## comments" in https:##sagecell.sagemath.org/
## Stark curve parameters extracted from https:##github.com/starkware-libs/cairo-lang/blob/master/src/starkware/cairo/common/ec.cairo


def StringToHex(String):
	return String.encode('utf-8').hex();

def CairoDeclare_from_Point(comment, Point):
	print("local ", comment,":(felt, felt)=(", hex(Point[0]), ",",hex(Point[1]),");");
	return 0;

def CairoDeclare_from_sacalar(comment, Scalar):
	print("local ", comment,":(felt)=(", hex(Scalar), ");");
	return 0;


# this function produces a test vector for the Musig 2 core functions i.e veryfing g^s=XR^c
# the values are dummy values only aimed at validating the Musig2_Verif_core function
# the hash value is considered as Random Oracle output instead of true hash
def Product_Test_Vector_MusigCore(curve_characteristic, curve_a, curve_b, Gx, Gy, curve_Order, index):
	Fp=GF(curve_characteristic); 				#Initialize Prime field of Point
	Fq=GF(curve_Order);					#Initialize Prime field of scalars
	Curve=EllipticCurve(Fp, [curve_a, curve_b]);		#Initialize Elliptic curve
	c=lift(Fq.random_element());					#Consider hash as random oracle
	s=lift(Fq.random_element());	
	curve_Generator=Curve([Gx, Gy]); 				
	Gpows=s*curve_Generator;
	
	R=Curve.random_element();
	Rpowc=c*R;
		
	X=Gpows-Rpowc;						#Producing X compliant with validity formulae 
	
	CairoDeclare_from_Point("X_"+index+":",X);
	CairoDeclare_from_Point("R_"+index+":",R);
	CairoDeclare_from_sacalar("s_"+index+":",s);
	CairoDeclare_from_sacalar("c_"+index+":",s);
	
	return 0;

#Produce the test vectors using Stark Curve as defined at https://docs.starkware.co/starkex/stark-curve.html
#order extracted elsewhere, can be checked by order*Generator=infty point

def Gen_Testvector_Stark_Musig2(nb_vectors):
	p = p=2^251+17*2^192+1     
	is_prime(p); #should return true
	beta = 0x6f21413efbe40de150e596d72f7a8c5609ad26c15c915c1f4cdfcb99cee9e89
	Stark_order=0x800000000000010ffffffffffffffffb781126dcae7b2321e66a241adc64d2f
	GEN_X = 0x1ef15c18599971b7beced415a40f0c7deacfd9b0d1819e03d723d8bc943cfca;
	GEN_Y = 0x5668060aa49730b7be4801df46ec62de53ecd11abe43a32873000c36e8dc1f;
	for index in [1..nb_vectors]:	
		Product_Test_Vector_MusigCore(p,1, beta, GEN_X, GEN_Y, Stark_order, str(index) );
	return 0;

Gen_Testvector_Stark_Musig2(2);





