/// @ref gtx_io
/// @file glm/gtx/io.inl
/// @author Jan P Springer (regnirpsj@gmail.com)

#include <iomanip>                  // std::fixed, std::setfill<>, std::setprecision, std::right, std::setw
#include <ostream>                  // std::basic_ostream<>
#include "../gtc/matrix_access.hpp" // glm::col, glm::row
#include "../gtx/type_trait.hpp"    // glm::type<>

namespace glm {
    namespace io {
        template<typename CTy>
        GLM_FUNC_QUALIFIER format_punct<CTy>::format_punct(size_t a)
                : std::locale::facet(a),
                  formatted(true),
                  precision(3),
                  width(1 + 4 + 1 + precision),
                  separator(','),
                  delim_left('['),
                  delim_right(']'),
                  space(' '),
                  newline('\n'),
                  order(column_major) {
        }

        template<typename CTy>
        GLM_FUNC_QUALIFIER format_punct<CTy>::format_punct(format_punct const &a)
                : std::locale::facet(0),
                  formatted(a.formatted),
                  precision(a.precision),
                  width(a.width),
                  separator(a.separator),
                  delim_left(a.delim_left),
                  delim_right(a.delim_right),
                  space(a.space),
                  newline(a.newline),
                  order(a.order) {
        }

        template<typename CTy> std::locale::id format_punct<CTy>::id;

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER basic_state_saver<CTy, CTr>::basic_state_saver(std::basic_ios<CTy, CTr> &a)
                : state_(a),
                  flags_(a.flags()),
                  precision_(a.precision()),
                  width_(a.width()),
                  fill_(a.fill()),
                  locale_(a.getloc()) {
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER basic_state_saver<CTy, CTr>::~basic_state_saver() {
            state_.imbue(locale_);
            state_.fill(fill_);
            state_.width(width_);
            state_.precision(precision_);
            state_.flags(flags_);
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER basic_format_saver<CTy, CTr>::basic_format_saver(std::basic_ios<CTy, CTr> &a)
                : bss_(a) {
            a.imbue(std::locale(a.getloc(), new format_punct<CTy>(get_facet < format_punct < CTy > > (a))));
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER
        basic_format_saver<CTy, CTr>::~basic_format_saver() {
        }

        GLM_FUNC_QUALIFIER precision::precision(unsigned a)
                : value(a) {
        }

        GLM_FUNC_QUALIFIER width::width(unsigned a)
                : value(a) {
        }

        template<typename CTy>
        GLM_FUNC_QUALIFIER delimeter<CTy>::delimeter(CTy a, CTy b, CTy c)
                : value() {
            value[0] = a;
            value[1] = b;
            value[2] = c;
        }

        GLM_FUNC_QUALIFIER order::order(order_type a)
                : value(a) {
        }

        template<typename FTy, typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER FTy const &_facet(std::basic_ios<CTy, CTr> &ios) {
            if (!std::has_facet<FTy>(ios.getloc()))
                ios.imbue(std::locale(ios.getloc(), new FTy));

            return std::use_facet<FTy>(ios.getloc());
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER std::basic_ios<CTy, CTr> &formatted(std::basic_ios<CTy, CTr> &ios) {
            const_cast<format_punct <CTy> &>(get_facet < format_punct < CTy > > (ios)).formatted = true;
            return ios;
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER std::basic_ios<CTy, CTr> &unformatted(std::basic_ios<CTy, CTr> &ios) {
            const_cast<format_punct <CTy> &>(get_facet < format_punct < CTy > > (ios)).formatted = false;
            return ios;
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, precision const &a) {
            const_cast<format_punct <CTy> &>(get_facet < format_punct < CTy > > (os)).precision = a.value;
            return os;
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, width const &a) {
            const_cast<format_punct <CTy> &>(get_facet < format_punct < CTy > > (os)).width = a.value;
            return os;
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, delimeter <CTy> const &a) {
            format_punct <CTy> &fmt(const_cast<format_punct <CTy> &>(get_facet < format_punct < CTy > > (os)));

            fmt.delim_left = a.value[0];
            fmt.delim_right = a.value[1];
            fmt.separator = a.value[2];

            return os;
        }

        template<typename CTy, typename CTr>
        GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, order const &a) {
            const_cast<format_punct <CTy> &>(get_facet < format_punct < CTy > > (os)).order = a.value;
            return os;
        }
    } // namespace io

    namespace detail {
        template<typename CTy, typename CTr, template<typename, precision> class V, typename T, precision P>
        GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &
        print_vector_on(std::basic_ostream<CTy, CTr> &os, V<T, P> const &a) {
            typename std::basic_ostream<CTy, CTr>::sentry const cerberus(os);

            if (cerberus) {
                io::format_punct<CTy> const &fmt(io::get_facet<io::format_punct<CTy> >(os));

                length_t const &components(type<V, T, P>::components);

                if (fmt.formatted) {
                    io::basic_state_saver<CTy> const bss(os);

                    os << std::fixed << std::right << std::setprecision(fmt.precision) << std::setfill(fmt.space)
                       << fmt.delim_left;

                    for (length_t i(0); i < components; ++i) {
                        os << std::setw(fmt.width) << a[i];
                        if (components - 1 != i)
                            os << fmt.separator;
                    }

                    os << fmt.delim_right;
                } else {
                    for (length_t i(0); i < components; ++i) {
                        os << a[i];

                        if (components - 1 != i)
                            os << fmt.space;
                    }
                }
            }

            return os;
        }
    }//namespace detail

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tquat<T, P> const &a) {
        return detail::print_vector_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tvec1<T, P> const &a) {
        return detail::print_vector_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tvec2<T, P> const &a) {
        return detail::print_vector_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tvec3<T, P> const &a) {
        return detail::print_vector_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tvec4<T, P> const &a) {
        return detail::print_vector_on(os, a);
    }

    namespace detail {
        template<typename CTy, typename CTr, template<typename, precision> class M, typename T, precision P>
        GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &print_matrix_on(std::basic_ostream<CTy, CTr> &os, M<T, P> const &a) {
            typename std::basic_ostream<CTy, CTr>::sentry const cerberus(os);

            if (cerberus) {
                io::format_punct<CTy> const &fmt(io::get_facet<io::format_punct<CTy> >(os));

                length_t const &cols(type<M, T, P>::cols);
                length_t const &rows(type<M, T, P>::rows);

                if (fmt.formatted) {
                    os << fmt.newline << fmt.delim_left;

                    switch (fmt.order) {
                        case io::column_major: {
                            for (length_t i(0); i < rows; ++i) {
                                if (0 != i)
                                    os << fmt.space;

                                os << row(a, i);

                                if (rows - 1 != i)
                                    os << fmt.newline;
                            }
                        }
                            break;

                        case io::row_major: {
                            for (length_t i(0); i < cols; ++i) {
                                if (0 != i)
                                    os << fmt.space;

                                os << column(a, i);

                                if (cols - 1 != i)
                                    os << fmt.newline;
                            }
                        }
                            break;
                    }

                    os << fmt.delim_right;
                } else {
                    switch (fmt.order) {
                        case io::column_major: {
                            for (length_t i(0); i < cols; ++i) {
                                os << column(a, i);

                                if (cols - 1 != i)
                                    os << fmt.space;
                            }
                        }
                            break;

                        case io::row_major: {
                            for (length_t i(0); i < rows; ++i) {
                                os << row(a, i);

                                if (rows - 1 != i)
                                    os << fmt.space;
                            }
                        }
                            break;
                    }
                }
            }

            return os;
        }
    }//namespace detail

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat2x2<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat2x3<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat2x4<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat3x2<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat3x3<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat3x4<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat4x2<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat4x3<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(std::basic_ostream<CTy, CTr> &os, tmat4x4<T, P> const &a) {
        return detail::print_matrix_on(os, a);
    }

    namespace detail {
        template<typename CTy, typename CTr, template<typename, precision> class M, typename T, precision P>
        GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &print_matrix_pair_on(std::basic_ostream<CTy, CTr> &os, std::pair<M<T, P> const, M<T, P> const> const &a) {
            typename std::basic_ostream<CTy, CTr>::sentry const cerberus(os);

            if (cerberus) {
                io::format_punct<CTy> const &fmt(io::get_facet<io::format_punct<CTy> >(os));
                M<T, P> const &ml(a.first);
                M<T, P> const &mr(a.second);
                length_t const &cols(type<M, T, P>::cols);
                length_t const &rows(type<M, T, P>::rows);

                if (fmt.formatted) {
                    os << fmt.newline << fmt.delim_left;

                    switch (fmt.order) {
                        case io::column_major: {
                            for (length_t i(0); i < rows; ++i) {
                                if (0 != i)
                                    os << fmt.space;

                                os << row(ml, i) << ((rows - 1 != i) ? fmt.space : fmt.delim_right) << fmt.space
                                   << ((0 != i) ? fmt.space : fmt.delim_left) << row(mr, i);

                                if (rows - 1 != i)
                                    os << fmt.newline;
                            }
                        }
                            break;
                        case io::row_major: {
                            for (length_t i(0); i < cols; ++i) {
                                if (0 != i)
                                    os << fmt.space;

                                os << column(ml, i) << ((cols - 1 != i) ? fmt.space : fmt.delim_right) << fmt.space
                                   << ((0 != i) ? fmt.space : fmt.delim_left) << column(mr, i);

                                if (cols - 1 != i)
                                    os << fmt.newline;
                            }
                        }
                            break;
                    }

                    os << fmt.delim_right;
                } else {
                    os << ml << fmt.space << mr;
                }
            }

            return os;
        }
    }//namespace detail

    template<typename CTy, typename CTr, typename T, precision P>
    GLM_FUNC_QUALIFIER std::basic_ostream<CTy, CTr> &operator<<(
            std::basic_ostream<CTy, CTr> &os,
            std::pair<tmat4x4<T, P> const,
                    tmat4x4<T, P> const> const &a) {
        return detail::print_matrix_pair_on(os, a);
    }
}//namespace glm
