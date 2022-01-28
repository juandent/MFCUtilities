#pragma once



class RendimientoCompositeImpl
{
	friend class RendimientoComposite;
	inline	static auto query(int pk)
	{
		auto rows = Storage::getStorage().select(columns(
			alias_column<als_r>(&Rendimiento::id),
			alias_column<als_f>(&Fondo::nombre),
			alias_column<als_r>(&Rendimiento::fecha),
			alias_column<als_r>(&Rendimiento::rendimiento_unitario)),

			inner_join<als_f>(on(c(alias_column<als_r>(&Rendimiento::fkey_fondo)) == alias_column<als_f>(&Fondo::id))),

			where(is_equal(alias_column<als_r>(&Rendimiento::id), pk)),
			multi_order_by(order_by(alias_column<als_r>(&Rendimiento::fkey_fondo)), order_by(alias_column<als_r>(&Rendimiento::fecha)).desc()));

		return rows;
	}
};



class RendimientoComposite
{
	using result_set_type = decltype(RendimientoCompositeImpl::query(1));
	result_set_type result_set;
	using row_type = decltype(result_set[0]);
public:
	RendimientoComposite(int pk) : result_set{ RendimientoCompositeImpl::query(pk) } {}

	int get_id() const
	{
		return std::get<0>(result_set[0]);
	}
	std::string get_nombre_fondo() const
	{
		return std::get<1>(result_set[0]);
	}
	std::chrono::sys_days get_rendimiento_fecha() const
	{
		return std::get<2>(result_set[0]);
	}

	double get_rendimiento_unitario() const
	{
		return std::get<3>(result_set[0]);
	}

	void set_rendimiento_unitario(double rend)
	{
		Storage::getStorage().update_all(set(assign(&Rendimiento::rendimiento_unitario, rend)), where(is_equal(&Rendimiento::id, get_id())));
	}
};
