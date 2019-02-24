#include "stdafx.h"
#include "NeuralNetwork.h"

inline std::string& ltrim(std::string &str1)
{
	std::string::iterator p = std::find_if(str1.begin(), str1.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
	str1.erase(str1.begin(), p);
	return str1;
}

std::string& rtrim(std::string &str1)
{
	std::string::reverse_iterator p = std::find_if(str1.rbegin(), str1.rend(), std::not1(std::ptr_fun<int, int>(std::isspace)));
	str1.erase(p.base(), str1.end());
	return str1;
}

std::string& trim(std::string &str1)
{
	ltrim(rtrim(str1));
	return str1;
}


int cv_load_txt(std::string file_path, cv::Mat& mat)
{
	//mat clear
	mat = cv::Mat();

	std::ifstream file(file_path);
	if (!file.is_open())
	{
		return -1;
	}

	int mat_rows = 0;
	int mat_cols = 0;
	std::vector<double> data;

	std::string line;
	while (std::getline(file, line))
	{
		line = trim(line);
		if (0 == line.length())
		{
			break;
		}
		int cols = 0;
		std::istringstream stream(line);
		double x;
		while (stream >> x)
		{
			cols++;
			data.push_back(x);
		}

		mat_cols = cols > mat_cols ? cols : mat_cols;
		mat_rows++;
	}

	if (mat_rows > 0 && mat_cols > 0)
	{
		mat = cv::Mat(data, true);
		mat = mat.reshape(0, mat_rows);
	}

	return 0;
}

std::string cv_mat2string(cv::Mat mat)
{
	std::stringstream ss;

	ss << "cv::mat trace:" << std::endl;

	if (!mat.empty())
	{
		for (int i = 0; i < mat.rows; i++)
		{
			for (int j = 0; j < mat.cols; j++)
			{
				ss << mat.at<double>(i, j) << " ";
			}

			ss << std::endl;
		}
	}

	ss << "cv::mat trace end" << std::endl;

	std::string mat_info = ss.str();
	return mat_info;
}

int GetTheta(cv::Mat& Theta1, cv::Mat& Theta2)
{
	int ret = 0;

	ret = cv_load_txt("theta\\Theta1.txt", Theta1);
	if (0 != ret)
	{
		return ret;
	}

	ret = cv_load_txt("theta\\Theta2.txt", Theta2);
	if (0 != ret)
	{
		return ret;
	}

	return 0;
}

int GetData(cv::Mat& X, cv::Mat& y)
{
	//5000
	cv_load_txt("data\\X.txt", X);

	//5000
	cv_load_txt("data\\y.txt", y);

	return 0;
}

int sigmoid(cv::Mat z, cv::Mat& g)
{
	cv::Mat minus_z_exp;
	cv::exp(-z, minus_z_exp);

	g = (1.0 / (1.0 + minus_z_exp));
	return 0;
}
cv::Mat sigmoid(cv::Mat z)
{
	cv::Mat minus_z_exp;
	cv::exp(-z, minus_z_exp);

	cv::Mat g = (1.0 / (1.0 + minus_z_exp));
	return g;
}

//function [J, grad] = lrCostFunction(theta, X, y, lambda)
//m = length(y); % number of training examples
//
//% You need to return the following variables correctly
//J = 0;
//grad = zeros(size(theta));

//H = sigmoid(X*theta);
//T = y.*log(H) + (1 - y).*log(1 - H);
//J = -1 / m*sum(T) + lambda / (2 * m)*sum(theta(2:end). ^ 2);
//
//ta = [0; theta(2:end)];
//grad = X'*(H - y)/m + lambda/m*ta;

int Predict(cv::Mat X, cv::Mat& a3)
{
	cv::Mat Theta1;
	cv::Mat Theta2;
	GetTheta(Theta1, Theta2);

	int m = X.rows;
	int num_labels = Theta2.rows;
	//cv_mat2string(X);
	//cv_mat2string(Theta1);
	//cv_mat2string(Theta2);

	//OutputDebugString(_T(""));

	cv::Mat a1;
	cv::hconcat(cv::Mat::ones(m, 1, CV_64FC1), X, a1);
	//cv_mat2string(a1);

	cv::Mat a2;
	cv::hconcat(cv::Mat::ones(m, 1, CV_64FC1), sigmoid(a1*Theta1.t()), a2);
	//cv_mat2string(a2);

	a3 = sigmoid(a2*Theta2.t());

	//cv_mat2string(a3);
	return 0;
}