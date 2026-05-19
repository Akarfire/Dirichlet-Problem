import dirichletsolver

import math
from dataclasses import dataclass, field
import streamlit as st
import plotly.graph_objects as graph


# Problem Parameters
a = 1.0
b = 2.0
c = 2.0
d = 3.0

# Test Seidel
def test_analytical(x, y):
    return math.sin(math.pi * x * y)

def test_f(x, y):
    return math.pi * math.pi * (x * x + y * y) * math.sin(math.pi * x * y)

def test_mu1(y):
   return math.sin(math.pi * a * y)

def test_mu2(y):
   return math.sin(math.pi * b * y)

def test_mu3(x):
   return math.sin(math.pi * c * x)

def test_mu4(x):
   return math.sin(math.pi * d * x)

# ...


# STREAM LIT

# # Initialize session state
# if 'data' not in st.session_state:
#     st.session_state.data = None


# st.set_page_config(layout="wide")
# st.title('Задача Дирихле для уравнения Пуассона')

# # # Sidebar
# with st.sidebar:
#     st.header("Параметры")
    
#     # Input parameters
#     problem_select = st.selectbox(
#     "Выберите задачу",
#     (
#         "Первая тестовая",
#         "Первая основная",
#         "Смешанная тестовая, классич. аппрокс. ГУ",
#         "Смешанная основная, улучш. аппрокс. ГУ")
#     )
    
#     n = st.number_input("Кол-во узлов", value=5, min_value=2, step=1, key="n") - 1
    
#     # Build button
#     build_button = st.button("Решить", type="primary", width='stretch')


# # Build button press event
# if build_button:
    
#     problem_id = [  "Первая тестовая",
#                     "Первая основная",
#                     "Смешанная тестовая, классич. аппрокс. ГУ",
#                     "Смешанная основная, улучш. аппрокс. ГУ"
#                  ].index(problem_select)
    
#     with st.spinner("Вычисляем..."):
#         # try:
#             if problem_id == 0:
#                 k1 = test_k1
#                 k2 = test_k2
#                 q1 = test_q1
#                 q2 = test_q2
#                 f1 = test_f1
#                 f2 = test_f2
                
#                 v_vector = boundarysolver.solve_bvp(k1, k2, q1, q2, f1, f2, ksi, mu1, mu2, int(n))
#                 control_vector = [test_analytical(0.0 + i * (1 / n)) for i in range(n + 1)]
#                 control_graph = control_vector
            
#             elif problem_id == 1:
#                 k1 = main_k1
#                 k2 = main_k2
#                 q1 = main_q1
#                 q2 = main_q2
#                 f1 = main_f1
#                 f2 = main_f2
                
#                 v_vector = boundarysolver.solve_bvp(k1, k2, q1, q2, f1, f2, ksi, mu1, mu2, int(n))
#                 control_graph = boundarysolver.solve_bvp(k1, k2, q1, q2, f1, f2, ksi, mu1, mu2, int(n) * 2)
#                 control_vector = control_graph[::2]

#             elif problem_id == 2:
#                 k1 = test_k1
#                 k2 = test_k2
#                 q1 = test_q1
#                 q2 = test_q2
#                 f1 = test_f1
#                 f2 = test_f2
                
#                 v_vector = boundarysolver.solve_bvp_mixed_test_function(k1, k2, q1, q2, f1, f2, ksi, int(n), gamma1, gamma2, theta1, theta2)
#                 control_vector = [test_analytical_mixed(0.0 + i * (1 / n)) for i in range(n + 1)]
#                 control_graph = control_vector
                
#             elif problem_id == 3:
#                 k1 = main_k1
#                 k2 = main_k2
#                 q1 = main_q1
#                 q2 = main_q2
#                 f1 = main_f1
#                 f2 = main_f2
                
#                 v_vector = boundarysolver.solve_bvp_mixed_advanced_approximation(k1, k2, q1, q2, f1, f2, ksi, int(n), gamma1, gamma2, theta1, theta2)
#                 control_graph = boundarysolver.solve_bvp_mixed_advanced_approximation(k1, k2, q1, q2, f1, f2, ksi, int(n) * 2, gamma1, gamma2, theta1, theta2)
#                 control_vector = control_graph[::2]
            
#             error_eval_list = [abs(v_vector[i] - control_vector[i]) for i in range(n + 1)]
            
#             error = 0.0
#             max_error_x = 0.0
#             for i, e in enumerate(error_eval_list):
#                 if e > error:
#                     error = e
#                     max_error_x = 0.0 + i * (1 / n)
            
#             # Store in session state
#             st.session_state.data = {
#                 'problem' : problem_id,
#                 'n' : n,
#                 'v_vector' : v_vector,
#                 'control_vector' : control_vector,
#                 'control_graph' : control_graph,
#                 'error' : error,
#                 'max_error_x' : max_error_x,
#                 'error_eval_list' : error_eval_list
#             }
            
#         # except Exception as e:
#         #     st.error(f"Ошибка: {str(e)}")
#         #     st.session_state.data = None

# # Display plot if data is available
# if st.session_state.data is not None:
#     data = st.session_state.data
#     problem = data['problem']
#     n = data['n']
#     v_vector = data['v_vector']
#     control_vector = data['control_vector']
#     control_graph = data['control_graph']
#     error = data['error']
#     max_error_x = data['max_error_x']
#     error_eval_list = data['error_eval_list']
    
#     # PLOT
#     st.subheader("Графики")
#     fig = graph.Figure()
    
#     fig.add_trace(graph.Scatter(
#         x=[0 + i * (1 / (len(control_graph) - 1)) for i in range(len(control_graph))], y=control_graph, 
#         mode='lines', 
#         name= 'V2' if (problem == 1 or problem == 3) else 'U',
#         line=dict(color='rgb(75, 75, 255)')
#     ))
    
#     fig.add_trace(graph.Scatter(
#         x=[0 + i * (1 / n) for i in range(n + 1)], y=v_vector, 
#         mode='lines', 
#         name='V',
#         line=dict(color='rgb(255, 255, 255)')
#     ))
    
#     fig.update_layout(
#         title="Решение",
#         xaxis_title="X",
#         yaxis_title="Y",
#         hovermode='closest',
#         legend=dict(
#             yanchor="top",
#             y=1,
#             xanchor="left",
#             x=1.02,
#             bordercolor="black",
#             borderwidth=1
#         )
#     )
    
#     st.plotly_chart(fig, width='stretch')
    
#     fig = graph.Figure()
    
#     fig.add_trace(graph.Scatter(
#         x=[0 + i * (1 / n) for i in range(n + 1)], y=[control_vector[i] - v_vector[i] for i in range(n + 1)], 
#         mode='lines', 
#         name='Error'
#     ))
    
#     fig.update_layout(
#         title="Погрешность",
#         xaxis_title="X",
#         yaxis_title="Y",
#         hovermode='closest',
#         legend=dict(
#             yanchor="top",
#             y=1,
#             xanchor="left",
#             x=1.02,
#             bordercolor="black",
#             borderwidth=1
#         ),
#         yaxis=dict(
#             exponentformat='power', # Options: 'none', 'e', 'E', 'power', 'SI', 'B'
#             showexponent='all'      # Options: 'none', 'all', 'first', 'last'
#         )
#     )
    
#     st.plotly_chart(fig, width='stretch')
    
#     # INFO
#     st.subheader("Справка")
    
#     if problem == 0:
#         st.info(f"""Для решения задачи использована равномерная сетка с числом разбиений n = {n}; \n
# Задача должна быть решена с погрешностью не более ε = 0.5⋅10^(–6); \n
# Задача решена с погрешностью ε1 = {error}; \n
# Максимальное отклонение аналитического и численного решений наблюдается в точке x = {max_error_x}.
# """)
        
#     elif problem == 1:
#         st.info(f"""Для решения задачи использована равномерная сетка с числом разбиений n = {n}; \n
# Задача должна быть решена с погрешностью не более ε = 0.5⋅10^(–6); \n
# Задача решена с погрешностью ε2 = {error}; \n
# Максимальное отклонение аналитического и численного решений наблюдается в точке x = {max_error_x}.
# """)
    
#     elif problem == 2:
#         st.info(f"""Для решения задачи использована равномерная сетка с числом разбиений n = {n}; \n
# Задача должна быть решена с погрешностью не более ε = 0.5⋅10^(–6); \n
# Задача решена с погрешностью ε3 = {error}; \n
# Максимальное отклонение аналитического и численного решений наблюдается в точке x = {max_error_x}.
# """)
        
#     elif problem == 3:
#         st.info(f"""Для решения задачи использована равномерная сетка с числом разбиений n = {n}; \n
# Задача должна быть решена с погрешностью не более ε = 0.5⋅10^(–6); \n
# Задача решена с погрешностью ε4 = {error}; \n
# Максимальное отклонение аналитического и численного решений наблюдается в точке x = {max_error_x}.
# """)
    
#     # DATA
#     st.subheader("Таблица")
    
#     # Table data
#     if problem == 0 or problem == 2:
#         table_data = []
#         for i in range(n + 1):
#             table_data.append({
#                 "N": i,
#                 "X_i": 0.0 + i * (1 / n),
#                 "U_i": control_vector[i],
#                 "V_i": v_vector[i],
#                 "U_i - V_i": control_vector[i] - v_vector[i],
#             })
            
#         column_config = {
#             "N": st.column_config.NumberColumn(
#                 "N"
#             ),
#             "X_i": st.column_config.NumberColumn(
#                 "X_i",
#                 format="%.5f"
#             ),
#             "U_i": st.column_config.NumberColumn(
#                 "U_i",
#                 format="%.5f"
#             ),
#             "V_i": st.column_config.NumberColumn(
#                 "V_i",
#                 format="%.5f"
#             ),
#             "U_i - V_i": st.column_config.NumberColumn(
#                 "U_i - V_i",
#                 format="%.15f"
#             )
#         }
        
#         st.dataframe(table_data, width='stretch', column_config=column_config)
        
        
#     elif problem == 1 or problem == 3:
#         table_data = []
#         for i in range(n + 1):
#             table_data.append({
#                 "N": i,
#                 "X_i": 0.0 + i * (1 / n),
#                 "V_i": v_vector[i],
#                 "V2_i": control_vector[i],
#                 "V2_i - V_i": control_vector[i] - v_vector[i],
#             })
            
#         column_config = {
#             "N": st.column_config.NumberColumn(
#                 "N"
#             ),
#             "X_i": st.column_config.NumberColumn(
#                 "X_i",
#                 format="%.15f"
#             ),
#             "V_i": st.column_config.NumberColumn(
#                 "V_i",
#                 format="%.15f"
#             ),
#             "V2_i": st.column_config.NumberColumn(
#                 "V2_i",
#                 format="%.15f"
#             ),
#             "V2_i - V_i": st.column_config.NumberColumn(
#                 "V2_i - V_i",
#                 format="%.15f"
#             )
#         }
        
#         st.dataframe(table_data, width='stretch', column_config=column_config)