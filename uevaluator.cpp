#include "uevaluator.h"

UEvaluator::UEvaluator(){}

UEvaluator::~UEvaluator(){}

void UEvaluator::evalBezier(GMlib::DMatrix<float> &BHm, int d, float t, float scale) //global/local affine mapping - global/local scaling factor
{
    if (d < 1) // Escape if no derivatives has to be computed
    {
        BHm.setDim(1, 1);
        BHm[0][0] = float(1);
        return;
    }
    else
    {
        BHm.setDim(d + 1, d + 1); // Initiate result matrix
    }

    //Initialize with zeros
    for (int i = 0; i < BHm.getDim1(); i++)
    {
        for (int j = 0; j < BHm.getDim2(); j++)
        {
            BHm[i][j] = float(0);
        }
    }

    //First step, bhp polynomials up to degree d
    //One for each row, starting from the bottom up
    BHm[d-1][0] = 1 - t;
    BHm[d-1][1] = t;

    for(int i = d - 2; i >= 0; i--)
    {
        BHm[i][0] = (1-t) * BHm[i+1][0];

        for(int j = 1; j < d-i; j++)
        {
            BHm[i][j] = t * BHm[i+1][j-1] + (1-t) * BHm[i+1][j];
        }
        BHm[i][d-i] = t * BHm[i+1][d-i-1];
    }

    //Verify if values are correct for first step; rows summing to one.
    for (int i = 0; i < d; i++)
    {
        float sum = 0;
        for (int j = 0; j <= d; j++)
        {
            sum += BHm[i][j];
        }

        if (abs(sum) - 1 > 1e-5)
        {
            std::cout << "Step one: Row " << i << " mismatch: " << sum << std::endl;
        }
        //else std::cout << "Row " << i << " ... OK" << std::endl;
    }

    //Next step, compute all existing derivatives
    BHm[d][0] = -scale;
    BHm[d][1] = scale;

    for (int k = 2; k <= d; k++)
    {
        const float s = k * scale;

        for (int i = d; i > (d - k); i--)
        {
            BHm[i][k] = s * BHm[i][k-1];

            for (int j = k - 1; j > 0; j--)
            {
                BHm[i][j] = s * (BHm[i][j-1] - BHm[i][j]);
            }

            BHm[i][0] = -s * BHm[i][0];
        }
    }

    //Check if values are correct for second step; rows summing to 0.
    for (int i = 1; i <= d; i++)
    {
        float sum = 0;
        for (int j = 0; j <= d; j++)
        {
            sum += BHm[i][j];
        }
        if (abs(sum) > 1e-5 )
        {
            std::cout << "Step two: Row " << i << " mismatch: " << sum << std::endl;
        }
        //else std::cout << "Row " << i << " ... OK" << std::endl;
    }
}

void UEvaluator::evalBSpline(GMlib::DMatrix<float> &BHm, float t, int idx, GMlib::DVector<float> kv, float scale)
{ 
    const int d = (kv.getDim() + 1) / 3;
    BHm.setDim(d+1, d+1);

    // Storing translated and scaled t values
    // Transformed t value
    GMlib::DVector<float> w;

    // Compute the Bernztein polynomials 1 -> d.
    // One for each row, starting from the bottom up.
    BHm[d-1][0] = 1 - getW(t, idx, 1, kv);
    BHm[d-1][1] = getW(t, idx, 1, kv);

    for( int i = d - 2, d_c = 2; i >= 0; i--, d_c++ )
    {
        // Generate w
        w.setDim(d_c);
        for( int j = d_c; j > 0; j-- )
        {
            w[j-1] = getW(t, idx - ( d_c - j ), d_c, kv);
        }

        // Compute the bernstein polynomials
        // Compute the k b-spline
        BHm[i][0] = ( 1 - w[0]) * BHm[i+1][0];
        for( int j = 1; j < d - i; j++ )
        {
            BHm[i][j] = w[j-1] * BHm[i+1][j-1] + (1 - w[j]) * BHm[i+1][j];
        }
        BHm[i][d-i] = w[d_c-1] * BHm[i+1][d-i-1];
    }


    // Compute all the deriatives
    BHm[d][0] = -getWder(idx, 1, kv) * scale;
    BHm[d][1] = getWder(idx, 1, kv) * scale;

    for( int k = 2; k <= d; k++ )
    {
        for( int i = d, d_c = 2; i > d - k; i--, d_c++ )
        {

            // Generate w for the derivatives
            w.setDim(d_c);
            for( int j = 1; j <= d_c; j++ )
            {
                w[j-1] = k * getWder(idx - ( d_c - j ), d_c, kv) * scale;
            }

            // Complete the bernstein polynomials by adding the computation of derivatives
            BHm[i][k] = w[0] * BHm[i][k-1];
            for( int j = k - 1, idx_c = 1; j > 0; j--, idx_c++ )
            {
                BHm[i][j] = w[idx_c] * ( BHm[i][j-1] - BHm[i][j] );
            }
            BHm[i][0] *= - w[d_c-1];
        }
    }
}

float UEvaluator::getW(float t, int idx, int d, GMlib::DVector<float> kv)   //get t scale
{
    return (t - kv[idx]) / (kv[idx+d] - kv[idx]);   //(equation 5.43)(p.115) scale of the center
}

float UEvaluator::getWder(int idx, int d, GMlib::DVector<float> kv)         //omega val, scale der - diffiomorphism
{
    return float(1) / (kv(idx+d) - kv(idx));        //(equation 5.47)(p.121) scale of the derivatives
}
