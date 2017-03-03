$( document ).ready(function() {
    console.log("Run");
    genTable();
    $('#rst').click(function(){
        $('table').remove();
        genTable();
    })
    $('body').mousedown(function() {
        isMouseDown = true;
    })
    .mouseup(function() {
        isMouseDown = false;
    });
    $('#snd').click(function(){
        let data = [];
        $('td').each(function(){
            data.push($(this).attr('ledEnabled'));
        })
        console.log('sending: ' + data.join(""));

        sendData("matriz",data.join(""));
    });

    $("#boton_inicio").click(function(){
        $(".container-matriz").css("visibility","hidden");
        $(".container-web").css("visibility","visible");
    })
    $("#boton_matriz").click(function(){
        $(".container-matriz").css("visibility","visible");
        $(".container-web").css("visibility","hidden");
    })

    $('#b_red').click(function(){
        sendData("led","red");
    });
    $('#b_green').click(function(){
        sendData("led","green");
    });
    $('#b_blue').click(function(){
        sendData("led","blue");
    });
    $('#b_off').click(function(){
        sendData("led","off");
    });
    $('#b_buzzer_on').click(function(){
        sendData("buzzer","ON");
    });
    $('#b_buzzer_off').click(function(){
        sendData("buzzer","OFF");
    });

    $('#b_matriz_bd').click(function(){
        sendData("matriz","11001101010101110010110101011100110");
    });
    $('#b_matriz_corazon').click(function(){
        sendData("matriz","01000101110111011111000111000001000");
    });
    $('#b_matriz_carita').click(function(){
        sendData("matriz","00101000010100100000101000100011100");
    });
    $('#b_matriz_one').click(function(){
        sendData("matriz","00000100100110111001001000100000111");
    });
    $('#matriznumb').click(function(){
        sendData("lcd",$('#matriznumval').val());
    });

    $(".iimg").click(function(){
        secret++;
    });

    $(".logo").click(function(){
        if(secret>=10){
            $(".bbuuzzeerr").css("display","block");
        }
    })



    document.getElementById('matriznumval').addEventListener('keydown', function(e) {
    var key   = e.keyCode ? e.keyCode : e.which;

  if (!([8, 9, 13, 27, 46, 110, 190].indexOf(key) !== -1 ||
      (key == 65 && (e.ctrlKey || e.metaKey)) ||
      (key >= 35 && key <= 40) ||
      (key >= 48 && key <= 57 && !(e.shiftKey || e.altKey)) ||
      (key >= 96 && key <= 105)
    )) e.preventDefault();
   else if ($('#matriznumval').val().length == 2) {
   		$('#matriznumval').val("");
   }
});

});



/////////////////////////////////////////////

//Configure
var rows = 5;
var columns = 7;
var server = "http://eldoctordeldesierto.com:9999/";
//var server = "http://127.0.0.1:9999/";

//other
var isMouseDown = false;
var secret = 0;

function sendData(type,datos){
    let data = {
        type: type,
        data: datos,
        id: $("#pinchiid").val()
    };
    $.get( server + "data/" + encodeURIComponent(JSON.stringify(data)) , function( res ) {
            console.log('server response: '+ res);
    });
}

// Generate Table
/*
    [][][][][][]
    [][][][][][]
*/
function genTable(){
    let row, column;
    var table = $('<table></table>').addClass('matriz');
    for(row=rows-1;row>=0;row--){
        var newRow = $('<tr></tr>');
        for(column=0;column<columns;column++){
            var newData = $('<td></td>').addClass('notselected').attr({
                row: row,
                column: column,
                id: String(row)+String(column),
                ledEnabled: 0
            });
            /*newData.mouseover(function(){
                if(!isMouseDown) return;
                toggle(this);
            });*/
            newData.click(function(){
                toggle(this);
            });
            
            newRow.append(newData);
        }
        table.append(newRow);
    }
    $('.grid').append(table);
}

function toggle(sel){
    
    if ( $(sel).attr('ledEnabled') == 0 ){
        $(sel).attr('ledEnabled','1');
        $(sel).addClass('ledEnabled');
    }
    else{
        $(sel).attr('ledEnabled','0');
        $(sel).removeClass('ledEnabled');
    }
}