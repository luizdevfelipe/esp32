#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "TicTacESP";
const char* password = "12345678";

WebServer server(80);

String pagina = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>TicTacESP</title>

<style>
*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Arial;
}

body{
height:100vh;
background:linear-gradient(135deg,#050816,#0f172a,#111827);
display:flex;
justify-content:center;
align-items:center;
color:white;
}

.container{
width:420px;
background:rgba(255,255,255,0.05);
border-radius:25px;
padding:30px;
text-align:center;
backdrop-filter:blur(10px);
box-shadow:0 0 30px rgba(0,255,255,0.2);
}

h1{
color:#00ffff;
font-size:40px;
margin-bottom:10px;
}

.subtitle{
color:#ccc;
margin-bottom:20px;
}

.scoreboard{
display:flex;
gap:10px;
margin-bottom:20px;
}

.score{
flex:1;
background:rgba(255,255,255,0.08);
padding:15px;
border-radius:15px;
}

.score span{
display:block;
margin-top:5px;
font-size:30px;
font-weight:bold;
}

.turn{
margin-bottom:20px;
font-size:22px;
color:#00ffff;
}

.board{
display:grid;
grid-template-columns:repeat(3,1fr);
gap:12px;
}

.cell{
width:110px;
height:110px;
background:rgba(255,255,255,0.08);
border-radius:20px;
display:flex;
justify-content:center;
align-items:center;
font-size:50px;
cursor:pointer;
transition:0.2s;
}

.cell:hover{
transform:scale(1.05);
background:rgba(0,255,255,0.1);
}

.x{
color:#00ffff;
}

.o{
color:#ff00ff;
}

button{
margin-top:20px;
width:100%;
padding:15px;
border:none;
border-radius:15px;
background:linear-gradient(90deg,#00ffff,#00ff99);
font-size:18px;
font-weight:bold;
cursor:pointer;
}

.message{
margin-top:20px;
font-size:24px;
color:#00ff99;
}
</style>
</head>

<body>

<div class="container">

<h1>TicTacESP</h1>

<div class="subtitle">Melhor de 3</div>

<div class="scoreboard">
<div class="score">Jogador X <span id="scoreX">0</span></div>
<div class="score">Jogador O <span id="scoreO">0</span></div>
</div>

<div class="turn" id="turn">Vez do jogador X</div>

<div class="board" id="board"></div>

<div class="message" id="message"></div>

<button onclick="resetGame()">Reiniciar</button>

</div>

<script>

const board = document.getElementById("board");
let currentPlayer = "X";
let cells = ["","","","","","","","",""];
let scoreX = 0;
let scoreO = 0;
let gameOver = false;

function createBoard(){

board.innerHTML="";

cells.forEach((cell,index)=>{

const div=document.createElement("div");

div.classList.add("cell");

if(cell=="X") div.classList.add("x");
if(cell=="O") div.classList.add("o");

div.innerText=cell;

div.addEventListener("click",()=>play(index));

board.appendChild(div);

});
}

function play(index){

if(cells[index]!==""||gameOver)return;

cells[index]=currentPlayer;

createBoard();

if(checkWinner()){

document.getElementById("message").innerText=
"Jogador "+currentPlayer+" venceu!";

gameOver=true;

if(currentPlayer=="X"){
scoreX++;
document.getElementById("scoreX").innerText=scoreX;
}else{
scoreO++;
document.getElementById("scoreO").innerText=scoreO;
}

if(scoreX==3||scoreO==3){

setTimeout(()=>{

alert("🏆 Jogador "+currentPlayer+" venceu o Melhor de 3!");

scoreX=0;
scoreO=0;

document.getElementById("scoreX").innerText=0;
document.getElementById("scoreO").innerText=0;

resetBoard();

},500);
}

return;
}

if(cells.every(cell=>cell!="")){
document.getElementById("message").innerText="Empate!";
gameOver=true;
return;
}

currentPlayer=currentPlayer=="X"?"O":"X";

document.getElementById("turn").innerText=
"Vez do jogador "+currentPlayer;

}

function checkWinner(){

const wins=[
[0,1,2],[3,4,5],[6,7,8],
[0,3,6],[1,4,7],[2,5,8],
[0,4,8],[2,4,6]
];

return wins.some(combo=>{
return combo.every(index=>{
return cells[index]==currentPlayer;
});
});
}

function resetBoard(){

cells=["","","","","","","","",""];

currentPlayer="X";
gameOver=false;

document.getElementById("message").innerText="";
document.getElementById("turn").innerText="Vez do jogador X";

createBoard();
}

function resetGame(){

scoreX=0;
scoreO=0;

document.getElementById("scoreX").innerText=0;
document.getElementById("scoreO").innerText=0;

resetBoard();
}

createBoard();

</script>

</body>
</html>
)rawliteral";

void paginaInicial(){
server.send(200,"text/html",pagina);
}

void setup(){

Serial.begin(115200);

WiFi.softAP(ssid,password);

Serial.println("");
Serial.println("WiFi criado!");
Serial.print("IP: ");
Serial.println(WiFi.softAPIP());

server.on("/",paginaInicial);

server.begin();

Serial.println("Servidor iniciado!");
Serial.println("192.168.4.1");

}

void loop(){
server.handleClient();
}