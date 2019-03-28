//
// Created by leezear on 18-4-6.
//

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <numeric>
#include <valarray>
#include <iostream>
#include <climits>
#include <algorithm>

namespace cudacp {

	const int MAX_VALUE = INT_MAX - 4096;
	const int MAX_OPT = INT_MIN + 4096;
	const int MIN_USER_OPT = MAX_OPT - 1024;

	enum ExpType {
		ET_OP,
		ET_CONST,
		ET_VAR,
		ET_NONE = INT_MIN,
		ET_LPAR = INT_MIN + 1,
		ET_RPAR = INT_MIN + 2,
		ET_COMMA = INT_MIN + 3,
		ET_PARAMS = MAX_OPT,
		ET_MARK,
		ET_NULL
	};

	enum ConType {
		CT_EXT, CT_INT
	};

	using namespace std;

	class HVar {
	public:
		int id;
		int uid;
		string name;
		vector<int> vals;
		unordered_map<int, int> val_map;
		vector<int> anti_map;
		const int std_min = 0;
		const int std_max;
		bool is_std = false;

		HVar(const int id, const int uid, string name, const int min_val, const int max_val);

		HVar(const int id, const int uid, const string name, vector<int>& v);

		~HVar();

		void show();
	};

	class HTab {
	public:
		int id;
		vector<HVar*> scope;
		bool isSTD = false;
		bool semantics;
		vector<vector<int>> tuples;

		HTab(const int id, const bool sem, vector<vector<int>>& ts, vector<HVar*>& scp, const bool STD);
		HTab(const int id, const bool sem, vector<vector<int>>& ts, vector<HVar*>& scp, const bool transform, const bool STD);
		HTab(HTab* t, vector<HVar*>& scp);

		int GetAllSize() const;

		void GetSTDTuple(vector<int>& src_tuple, vector<int>& std_tuple);

		void ToSTDTuple(vector<int>& ori_t, vector<int>& std_t);

		void GetORITuple(vector<int>& std_tuple, vector<int>& ori_tuple);

		bool SAT(vector<int>& t);

		bool SAT_STD(vector<int>& t);

		void show();

		void GetTuple(int idx, vector<int>& t, vector<int>& t_idx);

	private:
		//临时变量
		vector<int> tmp_t_;

		friend ostream& operator<<(ostream& os, const vector<HVar*>& a);
	};

	class HModel {
	public:
		vector<HVar*> vars;
		vector<HTab*> tabs;
		//unordered_map<HVar *, vector<HTab *>> subscriptions;
		////两个矩阵组成的矩阵,矩阵内容为，作用在两个变量之间约束的个数
		////.empty()表示无约束作用在该两个变量之间
		//vector<vector<vector<int>>> neighborhoods;

		HModel();


		HModel(bool transform);
		virtual ~HModel();

		int AddVar(const string& name, const int min_val, const int max_val);

		int AddVar(const string& name, vector<int>& v);

		int AddTab(const bool sem, vector<vector<int>>& ts, vector<HVar*>& scp, const bool STD = false);

		int AddTab(const bool sem, vector<vector<int>>& ts, vector<string>& scp);

		//int AddTab(const string expr);

		int AddTabAsPrevious(HTab* t, vector<string>& scp);

		int max_domain_size() const { return mds_; }

		int max_arity() const { return mas_; }

		void show();

		bool is_positive() const;

		//int regist(const string& exp_name, function<int(vector<int> &)>);

		//static int calculate(vector<int> &stack, vector<int> &params_len);

	private:
		//void get_postfix(const string expr, vector<int> &data, vector<int> &params, vector<int> &num_op_params,
		//                 vector<HVar *> &scp);

		//tuple<ExpType, int> get_type(string expr);

		//static ExpType get_type(const int expr);

		//void subscript(HTab *t);

		//void neighbor(HTab *t);

		void get_scope(vector<string>& scp_str, vector<HVar*>& scp);

		int get_var_id(const int id) const;

		int generate_exp_uid();

		int generate_var_uid();

		static void GetSTDTuple(vector<int>& src_tuple, vector<int>& std_tuple, vector<HVar*>& scp);

		static void GetORITuple(vector<int>& std_tuple, vector<int>& ori_tuple, vector<HVar*>& scp);

		static void get_ori_tuple_by_index(int idx, std::vector<int>& t, const vector<HVar*> scp);

		static void result(int op, vector<int>& result, const int len);

		unordered_map<string, HVar*> str_var_map_;
		unordered_map<int, HVar*> int_var_map_;
		int mds_ = 0;
		int mas_ = 0;
		int exp_id_ = MIN_USER_OPT;
		int var_uid_ = MAX_VALUE;
		bool need_positive_ = false;
	};

};

