#pragma once



template<typename Table, typename Source = Storage::Storage_t>
class SearchResults
{
	Source& m_source;
	std::vector<Table> m_results;
	int m_index = -1;
	CEdit& m_search_string;
public:

	SearchResults(Source& source, CEdit& search_string) : m_source{ source }, m_search_string(search_string) {}

	template<typename WhereClause, typename OrderByClause>
	void search(WhereClause w_clause, OrderByClause o_clause)
	{
		m_results = m_source.get_all<Table>(where(w_clause), order_by(o_clause));
		m_index = -1;
	}

	void set_search_string(const std::string& search)
	{
		m_search_string << search;
	}
	bool empty() const
	{
		return m_results.empty();
	}
	void clear()
	{
		m_results.clear();
		m_index = -1;
		m_search_string << "";
	}

	std::optional<Table> get_next_result(std::string Table::*text )
	{
		std::optional<Table> res;

		if (!m_results.empty())
		{
			if (m_index + 1 >= m_results.size())
			{
				m_index = 0;
			}
			else
			{
				m_index++;
			}
			res = m_results[m_index];

			auto table = *res;
			auto t = table.*text;

			m_search_string << t;


		}
		return res;
	}
};


