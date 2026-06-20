import dirichletsolver

import math
from dataclasses import dataclass, field
import streamlit as st
import plotly.graph_objects as go
import numpy as np

def calculate_residual(numerical_solution : list[list[float]], f : callable, a, b, c, d, n : int, m : int) -> list[list[float]]:
   residual = []
   residual.append([0 for j in range(0, m + 1)])
   h = (b - a) / n
   k = (d - c) / m
   inv_h_sq = 1 / h**2
   inv_k_sq = 1 / k**2
   for i in range(1, n):
      residual.append([0])
      for j in range(1, m):
         residual[i].append(0)
         residual[i][j] = inv_h_sq * (numerical_solution[i + 1][j] - 2 * numerical_solution[i][j] + numerical_solution[i - 1][j])
         residual[i][j] += inv_k_sq * (numerical_solution[i][j + 1] - 2 * numerical_solution[i][j] + numerical_solution[i][j - 1])
         residual[i][j] += f(a + i * h, c + j * k)

      residual[i].append(0)
   residual.append([0 for j in range(0, m + 1)])
   return residual


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


def main_f(x, y):
   return -1 * math.exp(-1 * x * y * y)

def main_mu1(y):
   return (y - 2) * (y - 3)

def main_mu2(y):
   return y * (y - 2) * (y - 3)

def main_mu3(x):
   return (x - 1) * (x - 2)

def main_mu4(x):
   return x * (x - 1) * (x - 2)
# ...

# Simple test
# x_step = (b - a) / n
# y_step = (d - c) / m
# errors = [[abs(solution[i][j] - test_analytical(a + x_step * i, c + y_step * j)) for j in range(m + 1)] for i in range(n + 1)]

# STREAM LIT

# Initialize session state
if 'data' not in st.session_state:
    st.session_state.data = None


st.set_page_config(layout="wide")
st.title('Задача Дирихле для уравнения Пуассона')

# Sidebar
with st.sidebar:
   st.header("Параметры")
   
   # Input parameters
   problem_select = st.selectbox(
   "Выберите задачу",
   (
      "Метод Зейделя Тестовая",
      "Метод Зейделя Основная",
      "Метод Верхней Релаксации Тестовая",
      "Метод Верхней Релаксации Основная")
   )
   
   n = st.number_input("Сетка n", value=5, min_value=2, step=1, key="n")
   m = st.number_input("Сетка m", value=5, min_value=2, step=1, key="m")
   
   iter_max = st.number_input("Макс. Итерация", value=10000, min_value=2, step=1, key="iter_max")
   epsilon = st.number_input("Эпсилон", value=1e-7, min_value=1e-15, step=0.0001, key="epsilon", format="%g")
   
   epsilon_2 = 0.0
   if problem_select == "Метод Зейделя Основная" or problem_select == "Метод Верхней Релаксации Основная":
      epsilon_2 = st.number_input("Эпсилон для контрольного решения", value=1e-9, min_value=1e-15, step=0.0001, key="epsilon_2", format="%g")
   
   omega = 0.0
   if problem_select == "Метод Верхней Релаксации Тестовая" or problem_select == "Метод Верхней Релаксации Основная":
      omega = st.number_input("Омега", value=1.87, min_value=0.000000000001, max_value=2.0, step=0.01, key="omega", format="%g")
      
   omega_2 = 0.0
   if (problem_select == "Метод Верхней Релаксации Основная"):
      omega_2 = st.number_input("Омега для контрольного решения", value=1.9, min_value=0.000000000001, max_value=2.0, step=0.01, key="omega_2", format="%g")
   
   # Build button
   build_button = st.button("Решить", type="primary", width='stretch')


# Build button press event
if build_button:
    
   problem_id = [   
                  "Метод Зейделя Тестовая",
                  "Метод Зейделя Основная",
                  "Метод Верхней Релаксации Тестовая",
                  "Метод Верхней Релаксации Основная"
                ].index(problem_select)
    
   with st.spinner("Вычисляем..."):
      x_step = (b - a) / n
      y_step = (d - c) / m
      
      c_iterations = 0
      c_epsilon_n = 0.0
      initial_approximation_2 = None
      if problem_id == 0:
         initial_approximation = dirichletsolver.get_initial_approximation(a, b, c, d, test_mu1, test_mu2, test_mu3, test_mu4, n, m)
         solution, iterations, epsilon_n = dirichletsolver.solve_seidel_method_main(
            test_f, a, b, c, d,
            test_mu1, test_mu2, test_mu3, test_mu4,
            n, m, iter_max, epsilon
         )
         control_graph = [[test_analytical(a + x_step * i, c + y_step * j) for j in range(m + 1)] for i in range(n + 1)]
         control_values = control_graph
         
      if problem_id == 1:
         initial_approximation = dirichletsolver.get_initial_approximation(a, b, c, d, main_mu1, main_mu2, main_mu3, main_mu4, n, m)
         solution, iterations, epsilon_n = dirichletsolver.solve_seidel_method_main(
            main_f, a, b, c, d,
            main_mu1, main_mu2, main_mu3, main_mu4,
            n, m, iter_max, epsilon
         )
         control_graph, c_iterations, c_epsilon_n = dirichletsolver.solve_seidel_method_main(
            main_f, a, b, c, d,
            main_mu1, main_mu2, main_mu3, main_mu4,
            n * 2, m * 2, iter_max, epsilon_2
         )
         control_values = [row[::2] for row in control_graph[::2]]
         initial_approximation_2 = dirichletsolver.get_initial_approximation(
               a, b, c, d, main_mu1, main_mu2, main_mu3, main_mu4, n * 2, m * 2
            )
         
         
      if problem_id == 2:
         initial_approximation = dirichletsolver.get_initial_approximation(a, b, c, d, test_mu1, test_mu2, test_mu3, test_mu4, n, m)
         solution, iterations, epsilon_n = dirichletsolver.solve_relax_method_main(
            test_f, a, b, c, d,
            test_mu1, test_mu2, test_mu3, test_mu4,
            n, m, iter_max, epsilon, omega
         )
         control_graph = [[test_analytical(a + x_step * i, c + y_step * j) for j in range(m + 1)] for i in range(n + 1)]
         control_values = control_graph
         
      if problem_id == 3:
         initial_approximation = dirichletsolver.get_initial_approximation(a, b, c, d, main_mu1, main_mu2, main_mu3, main_mu4, n, m)
         solution, iterations, epsilon_n = dirichletsolver.solve_relax_method_main(
            main_f, a, b, c, d,
            main_mu1, main_mu2, main_mu3, main_mu4,
            n, m, iter_max, epsilon, omega
         )
         control_graph, c_iterations, c_epsilon_n = dirichletsolver.solve_relax_method_main(
            main_f, a, b, c, d,
            main_mu1, main_mu2, main_mu3, main_mu4,
            n * 2, m * 2, iter_max, epsilon_2, omega_2
         )
         control_values = [row[::2] for row in control_graph[::2]]
         initial_approximation_2 = dirichletsolver.get_initial_approximation(
            a, b, c, d, main_mu1, main_mu2, main_mu3, main_mu4, n * 2, m * 2
         )
      
      error_eval_list = [[None] * (m + 1) for _ in range(n + 1)]
      for i in range(n + 1):
         for j in range(m + 1):
            error_eval_list[i][j] = abs(solution[i][j] - control_values[i][j])
      
      error = 0.0
      max_error_i = 0
      max_error_j = 0
      max_error_x = 0.0
      max_error_y = 0.0
      for i, row in enumerate(error_eval_list):
         for j, e in enumerate(row):
            if e >= error:
               error = e
               max_error_i = i
               max_error_j = j
               max_error_x = a + i * x_step
               max_error_y = c + j * y_step
      
      # Store in session state
      st.session_state.data = {
         'problem' : problem_id,
         'n' : n,
         'm' : m,
         'solution' : solution,
         'control_values' : control_values,
         'control_graph' : control_graph,
         'error' : error,
         'max_error_i' : max_error_i,
         'max_error_j' : max_error_j,
         'max_error_x' : max_error_x,
         'max_error_y' : max_error_y,
         'error_eval_list' : error_eval_list,
         'iterations' : iterations,
         'c_iterations' : c_iterations,
         'omega' : omega,
         'omega_2' : omega_2,
         'epsilon' : epsilon,
         'epsilon_2' : epsilon_2,
         'epsilon_n' : epsilon_n,
         'c_epsilon_n' : c_epsilon_n,
         'initial_approximation' : initial_approximation,
         'initial_approximation_2': initial_approximation_2
      }

# Display plot if data is available
if st.session_state.data is not None:
   data = st.session_state.data
   problem =  data['problem']
   n = data['n']
   m = data['m']
   solution = data['solution']
   control_values = data['control_values']
   control_graph = data['control_graph']
   error = data['error']
   max_error_i = data['max_error_i']
   max_error_j = data['max_error_j']
   max_error_x = data['max_error_x']
   max_error_y = data['max_error_y']
   error_eval_list = data['error_eval_list']
   iterations = data['iterations']
   c_iterations = data['c_iterations']
   omega = data['omega']
   oemga_2 = data['omega_2']
   epsilon = data['epsilon']
   epsilon_2 = data['epsilon_2']
   epsilon_n = data['epsilon_n']
   c_epsilon_n = data['c_epsilon_n']
   initial_approximation = data['initial_approximation']
   initial_approximation_2 = data['initial_approximation_2']
    
   # PLOT
   
   tab1, tab2, tab3, tab4 = st.tabs(["Численное решение", "Контрольное решение", "Погрешность", "Начальное приближение"])

   # Prepare coordinate grids
   x_step = (b - a) / n
   y_step = (d - c) / m
   x_coords = [a + i * x_step for i in range(n + 1)]
   y_coords = [c + j * y_step for j in range(m + 1)]

   # Prepare data for plotting (note: solution is indexed as [i][j])
   # Plotly expects z[y][x] format
   z_solution = [[solution[i][j] for i in range(n + 1)] for j in range(m + 1)]
   z_control = [[control_values[i][j] for i in range(n + 1)] for j in range(m + 1)]
   z_errors = [[error_eval_list[i][j] for i in range(n + 1)] for j in range(m + 1)]
   z_initial_approximation = [[initial_approximation[i][j] for i in range(n + 1)] for j in range(m + 1)]

   with tab1:
      st.subheader("3D Поверхность численного решения")
      
      # Create 3D surface plot
      fig_surface = go.Figure()
      
      # Add numerical solution surface
      fig_surface.add_trace(go.Surface(
         z=z_solution,
         x=x_coords,
         y=y_coords,
         colorscale='Viridis',
         name='Численное решение',
         showscale=True,
         colorbar=dict(title="u(x,y)")
      ))
      
      # Update layout
      fig_surface.update_layout(
         title='Решение задачи Дирихле',
         scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title='V(x,y)',
            camera=dict(
                  eye=dict(x=1.5, y=1.5, z=1.5)
            )
         ),
         width=800,
         height=600,
         margin=dict(l=0, r=0, b=0, t=30)
      )
      
      st.plotly_chart(fig_surface, use_container_width=True)

   with tab2:
      st.subheader("3D Поверхность контроля")
      
      # Create 3D surface plot
      fig_surface = go.Figure()
      
      # Add numerical solution surface
      fig_surface.add_trace(go.Surface(
         z=z_control,
         x=x_coords,
         y=y_coords,
         colorscale='Viridis',
         name='Поверхность контроля',
         showscale=True,
         colorbar=dict(title = "U(x,y)" if (problem == 0 or problem == 2) else "V2(x,y)")
      ))
      
      # Update layout
      fig_surface.update_layout(
         title='Решение задачи Дирихле',
         scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title="U(x,y)" if (problem == 0 or problem == 2) else "V2(x,y)",
            camera=dict(
                  eye=dict(x=1.5, y=1.5, z=1.5)
            )
         ),
         width=800,
         height=600,
         margin=dict(l=0, r=0, b=0, t=30)
      )
      
      st.plotly_chart(fig_surface, use_container_width=True)

   with tab3:
      st.subheader("3D Поверхность погрешности")
      
      # Error surface plot
      fig_error = go.Figure(data=go.Surface(
         z=z_errors,
         x=x_coords,
         y=y_coords,
         colorscale='RdYlGn_r',  # Red-Yellow-Green reversed
         name='Погрешность',
         colorbar=dict(title = "|V(x, y) - U(x,y)|" if (problem == 0 or problem == 2) else "|V(x, y) - V2(x,y)|")
      ))
      
      # Mark the maximum error point
      fig_error.add_trace(go.Scatter3d(
         x=[max_error_x],
         y=[max_error_y],
         z=[error],
         mode='markers',
         marker=dict(
            size=10,
            color='red',
            symbol='diamond'
         ),
         name=f'Макс. погрешность: {error:.2e}'
      ))
      
      fig_error.update_layout(
         title='Погрешность численного решения',
         scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title='Погрешность',
            camera=dict(
                  eye=dict(x=1.5, y=1.5, z=1.5)
            )
         ),
         width=800,
         height=600,
         margin=dict(l=0, r=0, b=0, t=30)
      )
      
      st.plotly_chart(fig_error, use_container_width=True)
   
   with tab4:
      st.subheader("3D График начального приближения")
      
      # Create 3D surface plot
      fig_surface = go.Figure()
      
      # Add numerical solution surface
      fig_surface.add_trace(go.Surface(
         z=z_initial_approximation,
         x=x_coords,
         y=y_coords,
         colorscale='Viridis',
         name='Поверхность контроля',
         showscale=True,
         colorbar=dict(title = "V0(x,y)")
      ))
      
      # Update layout
      fig_surface.update_layout(
         title='Решение задачи Дирихле',
         scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title="V0(x,y)",
            camera=dict(
                  eye=dict(x=1.5, y=1.5, z=1.5)
            )
         ),
         width=800,
         height=600,
         margin=dict(l=0, r=0, b=0, t=30)
      )
      
      st.plotly_chart(fig_surface, use_container_width=True)
   
   # INFO
   st.subheader("Справка")
   
   if problem == 0 or problem == 2:
      residual = max(abs(v) for row in calculate_residual(solution, test_f, a, b, c, d, n, m) for v in row)
      residual_init = max(abs(v) for row in calculate_residual(initial_approximation, test_f, a, b, c, d, n, m) for v in row)
      
         
   if problem == 1 or problem == 3:
      residual = max(abs(v) for row in calculate_residual(solution, main_f, a, b, c, d, n, m) for v in row)
      residual_2 = max(abs(v) for row in calculate_residual(control_graph, main_f, a, b, c, d, n * 2, m * 2) for v in row)
      residual_init = max(abs(v) for row in calculate_residual(initial_approximation, main_f, a, b, c, d, n, m) for v in row)
      residual_init_2 = max(abs(v) for row in calculate_residual(initial_approximation_2, main_f, a, b, c, d, n * 2, m * 2) for v in row)

   if problem == 0:
      st.info(f"""
      Для решения тестовой задачи использованы сетка с числом разбиений по x
      n = {n} и числом разбиений по y m = {m}, метод Зейделя, применены критерии остановки по 
      точности εмет = {epsilon} и по числу итераций Nmax = {iter_max}
      На решение схемы (СЛАУ) затрачено итераций N = {iterations} и достигнута точность 
      итерационного метода ε(N) = {epsilon_n}

      Схема (СЛАУ) решена с невязкой ||R(N)|| = {residual}
      для невязки СЛАУ использована норма «max»;
      
      Невязка начального приближения -- ||R0(N)|| = {residual_init}

      Тестовая задача должна быть решена с погрешностью не более ε = 0.5⋅10^(–6);
      задача решена с погрешностью ε1 = {error}.

      ---

      Максимальное отклонение точного и численного решений наблюдается в узле 
      x{max_error_i} = {max_error_x}; y{max_error_j} = {max_error_y};
      В качестве начального приближения использовано
      «Билинейная интерполяция по X, Y».
      """)
      
   if problem == 1:
      st.info(f"""
      Для решения основной задачи использована сетка с числом разбиений по x n = {n} 
      и числом разбиений по y m = {m}, метод Зейделя, 
      применены критерии остановки по точности εмет = {epsilon} и по числу итераций Nmax = {iter_max};

      На решение схемы (СЛАУ) затрачено итераций N = {iterations} и достигнута точность итерационного метода ε(N) = {epsilon_n};
      
      Схема (СЛАУ) решена с невязкой ||R(N)|| = {residual} использована норма «max»;
      
      Невязка начального приближения -- ||R0(N)|| = {residual_init}
   
 
      ---

      Для контроля точности решения использована сетка с половинным шагом, 
      метод Зейделя, 
      применены критерии остановки по точности 
      εмет-2 = {epsilon_2} и по числу итераций Nmax = {iter_max}

      На решение задачи (СЛАУ) затрачено итераций N2 = {c_iterations} 
      и достигнута точность итерационного метода ε(N2) = {c_epsilon_n}

      Схема (СЛАУ) на сетке с половинным шагом решена с невязкой
      ||R(N2)|| = {residual_2} использована норма «max»;
      
      Невязка начального приближения для сетки с половинным шагом -- ||R0(N2)|| = {residual_init_2}

      Основная задача должна быть решена с точностью не хуже чем
      ε = 0.5⋅10 –6; задача решена с точностью ε2 = {error}

      ---

      Максимальное отклонение точного и численного решений наблюдается в узле 
      x{max_error_i} = {max_error_x}; y{max_error_j} = {max_error_y}; 

      В качестве начального приближения на основной сетке использована «Билинейная интерполяция по X, Y»., 
      на сетке с половинным шагом использована «Билинейная интерполяция по X, Y».
      """)
      
   if problem == 2:
      st.info(f"""
      Для решения тестовой задачи использованы сетка с числом разбиений по x
      n = {n} и числом разбиений по y m = {m}, метод Верхней Релаксации с параметром ω = {omega}, применены критерии остановки по 
      точности εмет = {epsilon} и по числу итераций Nmax = {iter_max}
      На решение схемы (СЛАУ) затрачено итераций N = {iterations} и достигнута точность 
      итерационного метода ε(N) = {error}

      Схема (СЛАУ) решена с невязкой ||R(N)|| = {residual}
      для невязки СЛАУ использована норма «max»;
      
      Невязка начального приближения -- ||R0(N)|| = {residual_init}

      Тестовая задача должна быть решена с погрешностью не более ε = 0.5⋅10^(–6);
      задача решена с погрешностью ε1 = {error}.

      ---

      Максимальное отклонение точного и численного решений наблюдается в узле 
      x{max_error_i} = {max_error_x}; y{max_error_j} = {max_error_y};
      В качестве начального приближения использована
      «Билинейная интерполяция по X, Y».
      """)
      
   if problem == 3:
      st.info(f"""
      Для решения основной задачи использована сетка с числом разбиений по x n = {n} 
      и числом разбиений по y m = {m}, метод верхней релаксации с параметром ω = {omega}, 
      применены критерии остановки по точности εмет = {epsilon} и по числу итераций Nmax = {iter_max};

      На решение схемы (СЛАУ) затрачено итераций N = {iterations} и достигнута точность итерационного метода ε(N) = {epsilon_n};
      
      Схема (СЛАУ) решена с невязкой ||R(N)|| = {residual} использована норма «max»;
      
      Невязка начального приближения -- ||R0(N)|| = {residual_init}
      
      ---

      Для контроля точности решения использована сетка с половинным шагом, 
      метод верхней релаксации с параметром ω2 = {omega_2}, 
      применены критерии остановки по точности 
      εмет-2 = {epsilon_2} и по числу итераций Nmax = {iter_max}

      На решение задачи (СЛАУ) затрачено итераций N2 = {c_iterations} 
      и достигнута точность итерационного метода ε(N2) = {c_epsilon_n}

      Схема (СЛАУ) на сетке с половинным шагом решена с невязкой
      ||R(N2)|| = {residual_2} использована норма «max»
      
      Основная задача должна быть решена с точностью не хуже чем
      ε = 0.5⋅10 –6; задача решена с точностью ε2 = {error}

      ---

      Максимальное отклонение точного и численного решений наблюдается в узле 
      x{max_error_i} = {max_error_x}; y{max_error_j} = {max_error_y}; 


      В качестве начального приближения на основной сетке использована «Билинейная интерполяция по X, Y», 
      на сетке с половинным шагом использована «Билинейная интерполяция по X, Y».

      """)
   
   # DATA
   st.subheader("Решение")
   
   # Test tables

   table_data = [[None] * (n + 3) for _ in range(m + 3)]
   table_data[0] = ["", ""] + [f"x{i}" for i in range(n + 1)]
   table_data[1] = ["", "j/i"] + [str(i) for i in range(n + 1)]
   for j in range(2, m + 3):
      table_data[j][0] = f"y{j - 2}"
      table_data[j][1] = f"{j - 2}"
      for i in range(2, n + 3):
         table_data[j][i] = solution[i - 2][j - 2]

   st.dataframe(table_data, width='stretch')

   st.subheader("Точное решение" if problem == 0 or problem == 2 else "Решение с 2n, 2m")

   table_data = [[None] * (len(control_graph) + 2) for _ in range(len(control_graph[0]) + 2)]
   table_data[0] = ["", ""] + [f"x{i}" for i in range(len(control_graph))]
   table_data[1] = ["", "i/j"] + [str(i) for i in range(len(control_graph))]
   for j in range(2, len(control_graph[0]) + 2):
      table_data[j][0] = f"y{j - 2}"
      table_data[j][1] = f"{j - 2}"
      for i in range(2, len(control_graph) + 2):
         table_data[j][i] = control_graph[i - 2][j - 2]

   st.dataframe(table_data, width='stretch')
   
   st.subheader("Разность точного и контрольного решения")
   
   table_data = [[None] * (n + 3) for _ in range(m + 3)]
   table_data[0] = ["", ""] + [f"x{i}" for i in range(n + 1)]
   table_data[1] = ["", "i/j"] + [str(i) for i in range(n + 1)]
   for j in range(2, m + 3):
      table_data[j][0] = f"y{j - 2}"
      table_data[j][1] = f"{j - 2}"
      for i in range(2, n + 3):
         table_data[j][i] = error_eval_list[i - 2][j - 2]

   st.dataframe(table_data, width='stretch')
   