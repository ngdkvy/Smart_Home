// khoi tao cac thong so
let btn1 = document.querySelector('#btn1');
let img1 = document.querySelector('#call');
let btn2 = document.querySelector('#btn2');
// functions nut bam
// const database=firebase.database();
// const deviceRef=database.ref('quan1');

btn1.addEventListener('click', ()=>{
    img1.src = 'quatbat.gif'; 
    firebase.database().ref('thietbigiadung').set({quat: 1})
})
btn2.addEventListener('click', ()=>{
    img1.src = 'quattat.png';
    firebase.database().ref('thietbigiadung').set({quat: 0})
})


// khoi tao cac thong so
let btn3 = document.querySelector('#btn3');
let img2 = document.querySelector('#led');
let btn4 = document.querySelector('#btn4');
// functions nut bam
btn3.addEventListener('click', ()=>{
    img2.src = 'denon1.gif'; 
    firebase.database().ref('thietbigiadung').set({den: 1})
})

btn4.addEventListener('click', ()=>{
    img2.src = 'den1.png';
    firebase.database().ref('thietbigiadung').set({den: 0})
})

let btn5 = document.querySelector('#btn5');
let img3 = document.querySelector('#tv');
let btn6 = document.querySelector('#btn6');
// functions nut bam
btn5.addEventListener('click', ()=>{
    img3.src = 'tivibat.jpg'; 
    firebase.database().ref('thietbigiadung').set({tivi: 1})
})

btn6.addEventListener('click', ()=>{
    img3.src = 'tivitat.jpg';
    firebase.database().ref('thietbigiadung').set({tivi: 0})
})


